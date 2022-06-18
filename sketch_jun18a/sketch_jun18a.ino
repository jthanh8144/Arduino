#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

int ENA = 14;     // D5
int IN1 = 15;     // D10
int IN2 = 13;     // D7
int sensor = 12;  // D6

volatile unsigned int pulses;
float rpm;
unsigned long timeOld;
int holesDisc = 1;

void ICACHE_RAM_ATTR counter();

const char* ssid = "Really?";
const char* password = "1toi9boso8";
// const char* ssid = "jThanh8144";
// const char* password = "6789012345";
ESP8266WebServer server(80);

const byte PulsesPerRevolution = 2;
const unsigned long ZeroTimeout = 100000;
const byte numReadings = 2;

volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;

unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;
unsigned int ZeroDebouncingExtra;
unsigned long readings[numReadings];
unsigned long readIndex;
unsigned long total;
unsigned long average;

void ICACHE_RAM_ATTR Pulse_Event();

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
    // Serial.print("Speed: ");
    // Serial.println(speed);
    if (speed == 0) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      Serial.print("Speed: ");
      Serial.println(speed);
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", home);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/speed", getSpeed);
  server.on("/control", control);
  server.begin();

  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), Pulse_Event, RISING);

  Serial.println("setup done");
}

void loop() {
  server.handleClient();

  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  FrequencyReal = FrequencyRaw / 10000;

  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;
  total = total - readings[readIndex];
  readings[readIndex] = RPM;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
  Serial.print("\tRPM: ");
  Serial.print(RPM);
  Serial.println();
}

void ICACHE_RAM_ATTR Pulse_Event() {
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();
  if (PulseCounter >= AmountOfReadings)  {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;

    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  } else {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}


