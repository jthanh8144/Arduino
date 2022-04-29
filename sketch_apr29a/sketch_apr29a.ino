#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Khởi tạo RF24
RF24 radio(9, 8);  // CE, CSN

//Đặt địa chỉ phát.
const byte address[6] = "00001";
const int bientro = A0; // chọn chân biến trở là A0
int msg[3];// Mảng lưu giá trị

void setup()
{
  while (!Serial);
    Serial.begin(9600);
    
 radio.begin();
 radio.openWritingPipe(address);
 radio.stopListening();
}
void loop()
{
  int value = 86;
  msg[0] = value;
  radio.write(&value, sizeof(value));
  Serial.println(*msg); // xuất nội dung gửi đi lên Serial
  delay(1000);
}