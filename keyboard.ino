#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "keyboard";
const char* password = "password";

ESP8266WebServer server(80);

void handleRoot() {
  String clientIP = server.client().remoteIP().toString();
  Serial.print("Client connected from: ");
  Serial.println(clientIP);

  File file = SPIFFS.open("/index.html", "r");
  String html = file.readString();
  file.close();
  server.send(200, "text/html", "test?");
  //server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  SPIFFS.begin();


  WiFi.softAP(ssid, password);

  Serial.print("AP MODE SSID:");
  Serial.println(ssid);

  Serial.print("SERVER IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("INIT DONE");
}

void loop() {
  digitalWrite(2, HIGH);
  server.handleClient();
}