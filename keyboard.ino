#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid = "keyboard";
const char* password = "password";

ESP8266WebServer server(80);

const int LED_PIN = 2;
void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}


void handleRoot() {
  String clientIP = server.client().remoteIP().toString();
  Serial.print("Client connected from: ");
  Serial.println(clientIP);
  flashLED(3);
  File file = LittleFS.open("/index.html", "r");
  if(!file){
    Serial.println("ERROR LITTLEFS FILE");
    return;
  }
  String html = file.readString();
  file.close();
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  


  WiFi.softAP(ssid, password);

  Serial.print("AP MODE SSID:");
  Serial.println(ssid);

  Serial.print("SERVER IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  if(!LittleFS.begin()){
    Serial.println("LITTLEFS ERROR");
    return;
  }
  server.begin();
 
  Serial.println("INIT DONE");
}

void loop() {
  server.handleClient();
  digitalWrite(LED_PIN, LOW);
}