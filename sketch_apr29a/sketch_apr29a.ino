#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 8);
int btnPin = 2;
int ledPin = 7;

const byte address[6] = "00001";

void send() {
  delay(200);
  Serial.println("send");
  int value = 4;
  radio.write(&value, sizeof(value));
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
}

void setup()
{
  while (!Serial);
    Serial.begin(9600);
    
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();

  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(0, send, RISING);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("setup done");
}

void loop()
{
  
}
