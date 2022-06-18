#include <SoftwareSerial.h>

SoftwareSerial s(5,6);

int ENA = 14; // D5
int IN1 = 15; // D10
int IN2 = 13; // D7
int sensor = 2;

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
// unsigned long readings[numReadings];
// unsigned long readIndex;  
// unsigned long total; 
// unsigned long average;

int serialValue;

void left() {
  Serial.println("Turn left");
}

void right() {
  Serial.println("Turn right");
}

void control(int speed) {
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

void setup() {
  s.begin(115200);
  Serial.begin(9600);
  Serial.println();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), pulseEvent, RISING);
  delay(1000);
}

void loop() {
  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  FrequencyReal = FrequencyRaw / 10000;

  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;
  // total = total - readings[readIndex];
  // readings[readIndex] = RPM;
  // total = total + readings[readIndex];
  // readIndex = readIndex + 1;

  // if (readIndex >= numReadings) {
  //   readIndex = 0;
  // }
  // average = total / numReadings;

  // Serial.print("\tRPM: ");
  // Serial.println(RPM);

  if(s.available() > 0) {
    serialValue = s.read();
    if (serialValue >= 0 && serialValue <= 255) {
      control(serialValue);
    } else {
      switch (serialValue) {
        case 300:
          left();
          break;
        case 400:
          right();
          break;
        case 500:
          s.write(RPM);
          break;
        default:
          break;
      }
    }
  }
}

void pulseEvent() {
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
