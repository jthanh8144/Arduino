#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servo;

RF24 radio(9, 8);
const byte address[6] = "00001";
// int msg[3];
int msg;
bool status = false;

void setup()
{
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();

  servo.attach(3);
  servo.write(0);

  pinMode(2, INPUT);
  Serial.println("setup done");
}

void loop()
{
  Serial.println(digitalRead(2));
  if (digitalRead(2) == 0 && status == true) {
    for (int i = 0; i < 5; i++) {
      delay(1000);
      if (digitalRead(2) != 0) {
        break;
      }
    }
    Serial.println(digitalRead(2));
    if (digitalRead(2) == 0) {
      servo.write(0);
      status = false;
    }
  }
  if (radio.available())
  {
    while (radio.available()) {
      radio.read(&msg, sizeof(msg));
      Serial.println(msg);
      if (msg == 86) {
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
        // delay(1000);
        return;
      }
    }
  }
  // delay(1000);
}

void mainTask(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    // Serial.println("main");
    
  }
}

// void closeTask(void *pvParameters) {
//   (void) pvParameters;

//   for (;;) {
//     Serial.println(digitalRead(2));
//     if (digitalRead(2) == 0 && status == true) {
//       delay(2000);
//       if (digitalRead(2) == 0) {
//         servo.write(0);
//         status = false;
//       }
//     }
//     vTaskDelay(1);
//   }
// }
