#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

int ENA = 14; // D5
int IN1 = 15; // D10
int IN2 = 13; // D7
int sensor = 12; // D6

volatile unsigned int pulses;
float rpm;
unsigned long timeOld;
int holesDisc = 1;

void ICACHE_RAM_ATTR counter();

const char* ssid = "Really?";
const char* password = "1toi9boso8";
ESP8266WebServer server(80);

void home() {
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void left() {
  server.send(200, "application/json", "{\"status\": \"true\"}");

  Serial.println("left");
}

void right() {
  server.send(200, "application/json", "{\"status\": \"true\"}");

  Serial.println("right");
}

void getSpeed() {
  String json = "{\"speed\": \"" + String(rpm) + "\"}";
  server.send(200, "application/json", json);
}

void control() {
  if (server.arg("percent") == "") {
    Serial.println("receive error");
  } else {
    int speed = map(server.arg("percent").toInt(), 0, 100, 0, 255);
    Serial.print("Speed: ");
    Serial.println(speed);
    if (speed == 0) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
    }
  }
  server.send(200, "application/json", "{\"status\": \"true\"}");
}

void setup() {
  Serial.begin(9600);
  Serial.println();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

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
  server.on("/left", left);
  server.on("/right", right);
  server.on("/speed", getSpeed);
  server.on("/control", control);
  server.begin();

  pinMode(sensor, INPUT);
  pulses = 0;
  timeOld = 0;
  attachInterrupt(digitalPinToInterrupt(sensor), counter, FALLING);
}

void loop() {
  server.handleClient();

  if (millis() - timeOld >= 1000)
  {
    detachInterrupt(digitalPinToInterrupt(sensor));
    rpm = (pulses * 60) / (holesDisc);
    // Serial.println(rpm);
    
    timeOld = millis();
    pulses = 0;
    attachInterrupt(digitalPinToInterrupt(sensor), counter, FALLING);  
  }
}

void ICACHE_RAM_ATTR counter() {
  pulses++;
}
