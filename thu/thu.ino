#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <IRremote.h>

RF24 radio(9, 8);  // CE, CSN
Servo myservo;

const byte address[6] = "00001";
int msg[3];
bool status = false;

IRsend irsend;

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  
  myservo.attach(3);
  myservo.write(0);
}

void loop()
{
  irsend.sendSony(0xa90, 12);
  if (radio.available())
  {
      while (radio.available()){
        radio.read(&msg, sizeof(msg));
        Serial.print("Thu: ");
        Serial.println(msg[0]);
        Serial.println(msg[0] == 86);
        if (msg[0] == 86) {
          if (status == true)
          {
            myservo.write(0);
            status = false;
          }
          else
          {
            myservo.write(180);
            status = true;
          }
        }
        delay(1000);
     }
  }
  // delay(1000); 
}