#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ESP8266mDNS.h>

// upload datafolder to ESP with the plugin https://github.com/earlephilhower/arduino-esp8266littlefs-plugin in IDE version 1

// wifi AP mode
const char* ssid = "Remote Keyboard";
const char* password = "password";

ESP8266WebServer server(80);

// buildin led 2
const int LED_PIN = 2;

// flashled for debugging
void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

// handle webroot
void handleRoot() {
  String clientIP = server.client().remoteIP().toString();
  Serial.print("Client connected from: ");
  Serial.println(clientIP);
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
  
  // wifi
  WiFi.softAP(ssid, password, 6, false, 2); // ssid, password, channel, hidden, total connections
  Serial.print("AP MODE SSID:");
  Serial.println(ssid);
  Serial.print("SERVER IP: ");
  Serial.println(WiFi.softAPIP());
  

  // initialize LittleFS https://github.com/littlefs-project/littlefs
  if(!LittleFS.begin()){
    Serial.println("LITTLEFS ERROR");
    return;
  }

  // dns
  if (!MDNS.begin("keyboard")) {
    Serial.println("MDNS ERROR");
  }
  Serial.print("SERVER DNS: keyboard.local");

  // webserver
  server.on("/", handleRoot);

  // take flashled function javascript
  server.on("/flash-led", HTTP_GET, [](){
    String timesStr = server.arg("times");
    int times = timesStr.toInt();
    flashLED(times);
  });

  // start webserver (alwasy after the 'on' handlers)
  server.begin();
  Serial.println("INIT DONE");
}

void loop() {
  digitalWrite(LED_PIN, LOW);
  server.handleClient();
}