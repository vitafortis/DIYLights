#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
/*
 * As of now the code sets the PWM to the numbers given by the get request.(0-1024)
 * Ex: http://192.168.1.6/leds?R=0&G=512&B=1024
 */
ESP8266WebServer server(80);   //Web server object. Will be listening in port 80 (default for HTTP)
#define redPin 0       //Header Pin 3
#define greenPin 2     //Header Pin 4
#define bluePin 14     //Header Pin 5

void setup() {
Serial.begin(115200);
WiFi.begin("WIFI-NAME", "WIFI-PASSWORD"); //Connect to the WiFi network
while (WiFi.status() != WL_CONNECTED) { //Wait for connection
delay(500);
Serial.println("Connecting...");
}

Serial.print("IP address: ");
Serial.println(WiFi.localIP());
int zero = 0;
int half = 512;
setValues(zero,zero,zero);
delay(250);
setValues(half,half,half);
delay(250);
setValues(zero,zero,zero);
server.on("/leds", readValues);
server.begin();
Serial.println("Server listening");
}

void loop() {
server.handleClient();
}

void readValues() { 
int redValue = atoi(server.arg("R").c_str());
int greenValue = atoi(server.arg("G").c_str());
int blueValue = atoi(server.arg("B").c_str());

String message = "";
message = "Red Value = ";
message += redValue;
message += "\nGreen Value = ";
message += greenValue;
message += "\nBlue Value = ";
message += blueValue;
message += "\n";
setValues(redValue,greenValue,blueValue);
server.send(200, "text/plain", message);          //Returns the HTTP response

}

void setValues(int redValue, int greenValue, int blueValue){
analogWrite(redPin, redValue);
analogWrite(greenPin, greenValue);
analogWrite(bluePin, blueValue);
Serial.println("Setting Outputs:");
Serial.println(redValue);
Serial.println(greenValue);
Serial.println(blueValue);
}
