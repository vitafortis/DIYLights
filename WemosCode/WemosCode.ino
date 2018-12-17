#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
/*
 * As of now the code sets the PWM to the numbers given by the get request.(0-1024)
 * Ex: http://192.168.1.6/leds?R=0&G=512&B=1024
 * Es: http://192.168.1.6/leds?z=00&r=ff&g=ff&b=ff&t=ff
 */
ESP8266WebServer server(80);   //Web server object. Will be listening in port 80 (default for HTTP)
int pins[] = {D1, D2, D3, D4, D5, D6};

#define numOfStrips 2
#define tickLength 50
#define minTime 500
long last;

struct Light{
	int pins[3];
	int current[3];
	int next[3];
	int stepSize[3];
};

struct Light strips[numOfStrips];

void setup() {
	Serial.begin(115200);
  for( int i = 0; i < numOfStrips; i++){
    pinMode(pins[i*3], OUTPUT);
    pinMode(pins[i*3+1], OUTPUT);
    pinMode(pins[i*3+2], OUTPUT);
    strips[i].pins[0] = pins[i*3];
    strips[i].pins[1] = pins[i*3+1];
    strips[i].pins[2] = pins[i*3+2];
	}
 WiFi.mode(WIFI_STA);
	WiFi.begin("TRENDnet711", "gV3Rji8XfE"); //Connect to the WiFi network
	while (WiFi.status() != WL_CONNECTED) { //Wait for connection
		delay(500);
		Serial.println("Connecting...");
	}

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

  // ============ OTA =============
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
 
	server.on("/leds", readValues);
	server.begin();
	Serial.println("Server listening");
	last = millis();
}

void loop() {
	server.handleClient();
  ArduinoOTA.handle();
	if(last+tickLength <= millis()){
		updateAllLights();
		last = millis();
	}
}

void readValues() {
  int r = 0;
  int g = 0;
  int b = 0;
	int colVals[] = {0,0,0};
	int zone = atoi(server.arg("z").c_str());
	if ( zone != 0) {
  	r = atoi(server.arg("R").c_str());
  	g = atoi(server.arg("G").c_str());
  	b = atoi(server.arg("B").c_str());
		int time = atoi(server.arg("t").c_str());
		int ticks = (time*minTime);
		colVals[0] = map(r,0, 255, 0, 1023);
		colVals[1] = map(g,0, 255, 0, 1023);
		colVals[2] = map(b,0, 255, 0, 1023);
		if ( zone == 1) {
			setLights(0, colVals, ticks);
		} else if ( zone == 2) {
			setLights(1, colVals, ticks);
		} else if ( zone == 3 ) {
			setLights(0, colVals, ticks);
			setLights(1, colVals, ticks);
		}
	}
	String message = "";
	message = "Zone = ";
	message += server.arg("z").c_str();
	message += "\nRed Value = ";
	message += r;
	message += "\nGreen Value = ";
	message += g;
	message += "\nBlue Value = ";
	message += b;
	message += "\n";
	server.send(200, "text/plain", message);          //Returns the HTTP response

}

void setLights(int id, int vals[], int fade){
	for(int i=0;i<3;i++){
		strips[id].next[i] = vals[i];
	}
	if(fade != 0){
		for(int i=0;i<3;i++){
			strips[id].stepSize[i] = (strips[id].next[i]-strips[id].current[i])/(fade/tickLength);
		}
	}
	else{
		for(int i=0;i<3;i++){
			strips[id].current[i] = vals[i];
			analogWrite(strips[id].pins[i], vals[i]);
		}
	}
}

void updateAllLights(){
  for(int i=0;i<numOfStrips;i++){
		for(int j=0;j<3;j++){
      int current = strips[i].current[j];
      int next = strips[i].next[j];
			if(current != next){
				if(abs(current - next) < abs(strips[i].stepSize[j])){
					strips[i].current[j] = next;
				}
				else{
					strips[i].current[j] = current + strips[i].stepSize[j];
				}
		   analogWrite(strips[i].pins[j], strips[i].current[j]);
			}
		}
	}
}
