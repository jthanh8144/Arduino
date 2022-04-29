#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 #include <Servo.h>  // Thư viện điều khiển servo
 
// Servo myservo; // Khai báo đối tượng myservo dùng để điều khiển servo
//Khởi tạo đối tượng RF24 
RF24 radio(9, 8);  // CE, CSN
//Đặt địa chỉ nhận.
const byte address[6] = "00001";
int msg[3];// Mảng lưu các giá trị
void setup()
{
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  // myservo.attach(A0); // cài đặt chân A0 điều khiển servo   
}

void loop()
{
  //Đọc dữ liệu lưu vào buffer
  if (radio.available())
  {
      while (radio.available()){
          radio.read(&msg, sizeof(msg));
          // int servoPos = map(msg[0], 0, 1023, 0, 179); // hàm map chuyển giá trị 0 đến 1023 về 0 đến 179 của servo
          //  myservo.write(servoPos);  // Cho servo quay một góc là servoPos độ
           Serial.println(msg[0]); // xuất giá trị lên Serial
           delay(1000); // đợi 50ms cho servo chuyển góc
     }
  }
  delay(1000); 
}