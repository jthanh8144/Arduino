#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

SoftwareSerial s(D6, D5);

ESP8266WebServer server(80);

void home() {
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void left() {
  s.write(110);
  server.send(200, "application/json", "{\"status\": \"true\"}");
  Serial.println("left");
}

void right() {
  s.write(120);
  server.send(200, "application/json", "{\"status\": \"true\"}");
  Serial.println("right");
}

void getSpeed() {
  s.write(130);
  int speed = map(s.read(), 0, 255, 0, 10000);
  if (speed < 0) {
    speed = 0;
  }
  Serial.println("speed");
  String json = "{\"speed\": \"" + String(speed) + "\"}";
  server.send(200, "application/json", json);
}

void control() {
  Serial.println("control");
  if (server.arg("percent") == "") {
    Serial.println("receive error");
  } else {
    s.write(server.arg("percent").toInt());
  }
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void setup() {
  Serial.begin(9600);
  s.begin(115200);
  
  WiFi.softAP("boat");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println();

  server.on("/", home);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/speed", getSpeed);
  server.on("/control", control);
  server.begin();
  
  Serial.println("setup done");
}

void loop() {
  server.handleClient();
}
