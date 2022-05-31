#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <cstdlib>
#include <ctime>

const char* ssid = "Really?";
const char* password = "1toi9boso8";
ESP8266WebServer server(80);

void home() {
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void bat() {
  server.sendHeader("Location", "/", true);
  server.send(303);
  server.send(200, "application/json", "{\"status\": \"true\"}");
  Serial.println("bat");
}

void tat() {
  server.send(200, "application/json", "{\"status\": \"true\"}");
  Serial.println("tat");
}

void send() {
  srand(time(NULL));
	int res = rand();
  String json = "{\"temperature\": \"" + String(res) + "\"}";
  server.send(200, "application/json", json);
}

void receive() {
  if (server.arg("percent") == "") {
    Serial.println("receive error");
  } else {
    Serial.print("Percent is: ");
    Serial.println(server.arg("percent"));
  }
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("setup done");

  server.on("/", home);
  server.on("/up", bat);
  server.on("/down", tat);
  server.on("/send", send);
  server.on("/receive", receive);
  server.begin();
}

void loop() {
  server.handleClient();
  // delay(3000);
}
