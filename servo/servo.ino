#include <Servo.h> 
 
#define SERVO_PIN 11

Servo gServo;
 
void setup() 
{ Serial.begin(9600);
    gServo.attach(SERVO_PIN); 
    gServo.write(0);
    Serial.println("Setup");
}
 
void loop() 
{ 
    Serial.println("loop");
    gServo.write(0);
    delay(1000);
	
    // gServo.write(90);
    // delay(1000);
	
    gServo.write(180);
    delay(1000);
}