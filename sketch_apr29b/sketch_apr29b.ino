#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servo;

RF24 radio(9, 8);
int sensorPin = 2;
int btnPin = 3;
int servoPin = 6;

const byte address[6] = "00001";
int msg;
bool status = false;
bool autoClose = false;

void handle() {
  delay(200);
  autoClose = !autoClose;
  Serial.println("handle");
}

void setup()
{
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();

  servo.attach(servoPin);
  servo.write(0);

  pinMode(sensorPin, INPUT);

  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(1, handle, RISING);
  Serial.println("setup done");
}

void loop()
{
  if (autoClose == true) {
    // Serial.println(digitalRead(sensorPin));
    if (digitalRead(sensorPin) == 0 && status == true) {
      Serial.println(digitalRead(sensorPin));
      for (int i = 0; i < 5; i++) {
        delay(1000);
        if (digitalRead(sensorPin) != 0) {
          break;
        }
      }
      Serial.println(digitalRead(sensorPin));
      if (digitalRead(sensorPin) == 0) {
        servo.write(0);
        status = false;
      }
    }
  }
  
  if (radio.available())
  {
    while (radio.available()) {
      radio.read(&msg, sizeof(msg));
      Serial.println(msg);
      if (msg == 4) {
        if (status == true)
        {
          servo.write(0);
          status = false;
        }
        else
        {
          servo.write(160);
          status = true;
        }
        break;
      }
    }
  }
  // delay(1000);
}
