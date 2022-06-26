#include <SoftwareSerial.h>
#include <Servo.h> 

SoftwareSerial s(5,6);
Servo servo;

int IN1 = 8;
int IN2 = 9;
int ENA = 10;
int sensor = 2;
int servoPin = 11;
int startPos = 80;
int leftPos = 170;
int rightPos = 10;

const byte pulsesPerRevolution = 2;
const unsigned long zeroTimeout = 100000;
const byte numReadings = 2;
volatile unsigned long lastTimeWeMeasured;
volatile unsigned long periodBetweenPulses = zeroTimeout + 1000;
volatile unsigned long periodAverage = zeroTimeout + 1000;
unsigned long frequencyRaw;
unsigned long frequencyReal;
unsigned long RPM;
unsigned int pulseCounter = 1;
unsigned long periodSum;
unsigned long lastTimeCycleMeasure = lastTimeWeMeasured;
unsigned long currentMicros = micros();
unsigned int amountOfReadings = 1;
unsigned int zeroDebouncingExtra;

int serialValue;

void left() {
  Serial.println("Turn left");
  servo.write(leftPos);
  delay(500);
  servo.write(startPos);
}

void right() {
  Serial.println("Turn right");
  servo.write(rightPos);
  delay(500);
  servo.write(startPos);
}

void control(int percent) {
  int speed = map(percent, 0, 100, 0, 255);
  Serial.print("Speed in arduino: ");
  Serial.println(speed);
  if (speed == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println(speed);
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), pulseEvent, RISING);
  
  servo.attach(servoPin);
  servo.write(startPos);

  Serial.println("setup done");
}

void loop() {
  lastTimeCycleMeasure = lastTimeWeMeasured;
  currentMicros = micros();
  if (currentMicros < lastTimeCycleMeasure) {
    lastTimeCycleMeasure = currentMicros;
  }
  frequencyRaw = 10000000000 / periodAverage;
  if (periodBetweenPulses > zeroTimeout - zeroDebouncingExtra || currentMicros - lastTimeCycleMeasure > zeroTimeout - zeroDebouncingExtra) {
    frequencyRaw = 0;
    zeroDebouncingExtra = 2000;
  } else {
    zeroDebouncingExtra = 0;
  }
  frequencyReal = frequencyRaw / 10000;

  RPM = frequencyRaw / pulsesPerRevolution * 60;
  RPM = RPM / 10000;

  if(s.available() > 0) {
    serialValue = s.read();
    if (serialValue >= 0 && serialValue <= 100) {
      control(serialValue);
    } else {
      switch (serialValue) {
        case 110:
          left();
          break;
        case 120:
          right();
          break;
        case 130:
          int rpm = map(RPM, 0, 10000, 0, 255);
          s.write(rpm);
          Serial.print("\tRPM: ");
          Serial.print(RPM);
          Serial.print(" rpm send: ");
          Serial.println(rpm);
          break;
        default:
          break;
      }
    }
  }
}

void pulseEvent() {
  periodBetweenPulses = micros() - lastTimeWeMeasured;
  lastTimeWeMeasured = micros();
  if (pulseCounter >= amountOfReadings)  {
    periodAverage = periodSum / amountOfReadings;
    pulseCounter = 1;
    periodSum = periodBetweenPulses;

    int remapedAmountOfReadings = map(periodBetweenPulses, 40000, 5000, 1, 10);
    remapedAmountOfReadings = constrain(remapedAmountOfReadings, 1, 10);
    amountOfReadings = remapedAmountOfReadings;
  } else {
    pulseCounter++;
    periodSum = periodSum + periodBetweenPulses;
  }
}
