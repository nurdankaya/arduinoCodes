
//wifi Server  --> go to a given ip address /temperature and see the temperature values


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "espTest";
const char* password = "123456789";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  return "temperature: xx.xx";
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });

  
  
  // Start server
  server.begin();
}
 
void loop(){
  
}
