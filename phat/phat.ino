#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 8);  // CE, CSN
const byte address[6] = "00001";
int msg[3];

int ledPin = 7;
int button = 2;

void send() {
  int value = 86;
  msg[0] = value;
  radio.write(&value, sizeof(value));
  Serial.print("Phat: ");
  Serial.println(*msg);
  digitalWrite(ledPin, HIGH);
  delay(1000);
}

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
  pinMode(ledPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(0, send, RISING);
  digitalWrite(ledPin, LOW);
}
void loop()
{
  digitalWrite(ledPin, LOW);
  delay(500);
}
