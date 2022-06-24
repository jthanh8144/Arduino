#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

#include <cstdlib>
#include <ctime>

SoftwareSerial s(D6, D5);

// const char* ssid = "Really?";
// const char* password = "1toi9boso8";
const char* ssid = "jThanh8144";
const char* password = "6789012345";
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
  int rpm = s.read();
  // srand(time(NULL));
	// int rpm = rand();
  Serial.println("speed");
  String json = "{\"speed\": \"" + String(rpm) + "\"}";
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

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  WiFi.softAP("boat");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
  // Serial.print("Connected, IP address: ");
  // Serial.println(WiFi.localIP());
  Serial.println("setup done");

  server.on("/", home);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/speed", getSpeed);
  server.on("/control", control);
  server.begin();
}

void loop() {
  server.handleClient();
}
