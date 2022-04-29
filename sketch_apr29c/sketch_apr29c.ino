#include <IRremote.h>
IRsend irsend;
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN); 
decode_results results; 

void setup()
{
    Serial.begin(9600); // Initialize serial interface
    irrecv.enableIRIn();
    Serial.println("setup");
}
void loop() 
{
  // for (int i = 0; i < 3; i++) {
  //   irsend.sendRC5(0xA90, 12); //[12] Bit-length signal (hex A90=1010 1001 0000)
  //   // Serial.println("send");
  //   delay(40);
  // }
  if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX); 
		irrecv.resume(); // Receive the next value 
	}
	// delay (100);
}