#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "wifi.h"
#include "otaManager.h"

ESP8266WebServer server(80);
const int ledPin = D4; // LED pin
OTAManager ota;

void handleRoot() {
  server.send(200, "text/html", "<html><body><h1>ESP8266 Web Server</h1><button onclick=\"fetch('/toggle').then(response => response.text()).then(data => console.log(data))\">Toggle LED</button><div id='vu-meter'></div><script src='/vue.js'></script><script src='/app.js'></script></body></html>");
}

void handleToggleLED() {  
  digitalWrite(ledPin, !digitalRead(ledPin));
  server.send(200, "text/plain", "LED Toggled");
}

void handleVUData() {
  int vuValue = analogRead(A0); // Read VU meter value
  server.send(200, "application/json", String("{\"vu\":") + vuValue + "}");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  configWiFi(true, IPAddress(192, 168, 1, 34), IPAddress(192, 168, 1, 254), IPAddress(255, 255, 255, 0));
  
  ota.setupOTA("ESP8266", "admin");

  server.on("/", handleRoot);
  server.on("/toggle", handleToggleLED);
  server.on("/vu", handleVUData);
  server.serveStatic("/vue.js", SPIFFS, "/vue.js");
  server.serveStatic("/app.js", SPIFFS, "/app.js");

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  ota.handleOTA();
}
