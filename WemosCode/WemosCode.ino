#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
/*
 * As of now the code sets the PWM to the numbers given by the get request.(0-1024)
 * Ex: http://192.168.1.6/leds?R=0&G=512&B=1024
 * Es: http://192.168.1.6/leds?z=00&r=ff&g=ff&b=ff&t=ff
 */
ESP8266WebServer server(80);   //Web server object. Will be listening in port 80 (default for HTTP)
int pins[] = {D2, D3, D4, D5, D6, D7};
int lPins[] = {D2, D3, D4};
int rPins[] = {D5, D6, D7};

void setup() {
	Serial.begin(115200);
	for ( int i = 0; i < 6; i++) {
		pinMode(pins[i], OUTPUT);
	}
	WiFi.begin("Pixel_6651", "4ef84eb77ba1"); //Connect to the WiFi network
	while (WiFi.status() != WL_CONNECTED) { //Wait for connection
		delay(500);
		Serial.println("Connecting...");
	}

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	server.on("/leds", readValues);
	server.begin();
	Serial.println("Server listening");
}

void loop() {
	server.handleClient();
}

void readValues() { 
  int r = 0;
  int g = 0;
  int b = 0;
	int colVals[] = {0,0,0};
	int zone = atoi(server.arg("z").c_str());
	if ( zone == 0) {
		setValues(lPins, colVals);
		setValues(rPins, colVals);
	} else {


  r = atoi(server.arg("R").c_str());
  g = atoi(server.arg("G").c_str());
  b = atoi(server.arg("B").c_str());
  
	int time = atoi(server.arg("t").c_str()) / 2;

	colVals[0] = map(r,0, 255, 0, 1023);
	colVals[1] = map(g,0, 255, 0, 1023);
	colVals[2] = map(b,0, 255, 0, 1023);


	if ( zone == 1) {
		setValues(rPins, colVals);
	} else if ( zone == 10) {
		setValues(lPins, colVals);
	} else if ( zone == 11 ) {
		setValues(lPins, colVals);
		setValues(rPins, colVals);
	}
}

// int redValue = atoi(server.arg("R").c_str());
// int greenValue = atoi(server.arg("G").c_str());
// int blueValue = atoi(server.arg("B").c_str());

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
	//setValues(colVals);
	server.send(200, "text/plain", message);          //Returns the HTTP response

}

void setValues(int outPins[], int vals[]){
	Serial.println("Setting Outputs:");
	for ( int i = 0; i < 3; i++ ) {
		analogWrite(outPins[i], vals[i]);
		Serial.println(vals[i]);
	}
// analogWrite(redPin, redValue);
// analogWrite(greenPin, greenValue);
// analogWrite(bluePin, blueValue);
// Serial.println("Setting Outputs:");
// Serial.println(redValue);
// Serial.println(greenValue);
// Serial.println(blueValue);
}