#include <Servo.h> 
 
#define SERVO_PIN 6

Servo gServo;
 
void setup() 
{ 
    gServo.attach(SERVO_PIN); 
    gServo.write(0);
}
 
void loop() 
{ 
    // gServo.write(0);
    // delay(1000);
	
    // // gServo.write(90);
    // // delay(1000);
	
    // gServo.write(180);
    // delay(1000);
}