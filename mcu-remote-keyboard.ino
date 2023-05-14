#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include <USBHost.h>
#include <HIDProject.h>

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


// usb HID

USBHost usb;
HIDBoot<HID_PROTOCOL_KEYBOARD> keyboard(&usb);


void testHID(){
  usb.begin();
  while (!usb.isReady()) {} // Wait for USB to be ready
  keyboard.begin();
  keyboard.press(KEY_LEFT_GUI); // Press the Windows key
  keyboard.press('r'); // Press the 'r' key
  keyboard.releaseAll(); // Release all keys
  delay(1000); // Delay to prevent multiple keystrokes
  keyboard.print("Hello World"); // Send "Hello World" string
  delay(1000); // Delay to prevent multiple keystrokes
}

// reset back to programming mode
#define PROGRAMMING_PIN D3
void enterProgrammingMode() {
  pinMode(PROGRAMMING_PIN, OUTPUT);
  digitalWrite(PROGRAMMING_PIN, LOW);
  delay(500);
}
void exitProgrammingMode() {
  pinMode(PROGRAMMING_PIN, INPUT_PULLUP);
}
void reset(){
  Serial.begin(115200);
  delay(1000);
  Serial.println("Entering programming mode...");
  enterProgrammingMode();
  Serial.println("Programming mode entered!");
  delay(1000);
  Serial.println("Uploading sketch...");
  Serial.println("Sketch uploaded!");
  delay(1000);
  Serial.println("Exiting programming mode...");
  exitProgrammingMode();
  Serial.println("Programming mode exited!");
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

  // test usb keyboard
  server.on("/test-keyboard", HTTP_GET, [](){
    testHID();
  });

  // reset programming mode
  server.on("/reset", HTTP_GET, [](){
    reset();
  });

  // start webserver (alwasy after the 'on' handlers)
  server.begin();

  Serial.println("INIT DONE");
}

void loop() {
  server.handleClient();
}