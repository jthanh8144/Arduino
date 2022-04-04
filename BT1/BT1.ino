#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SIZE 2
String acceptCode[] = {"41 A8 8E 1C", "91 13 E5 1B"};
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;
bool status = false;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("\nApproximate your card to the reader...");

  myservo.attach(3);
  myservo.write(0);

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
}
void loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {    
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  Serial.print("Message : ");
  for (int i = 0; i < SIZE; i++) {
    if (content.substring(1) == acceptCode[i]) {
      Serial.println("Authorized access");
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
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
      delay(2000);
      return;
    }
  }
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
  Serial.println("Authorized denied");
  delay(2000);
}