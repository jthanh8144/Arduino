int ledPin11 = 11;
int ledPin12 = 12;

int value = 0;
int pinCount = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin11, OUTPUT);
  digitalWrite(ledPin11, HIGH);
  pinMode(ledPin12, OUTPUT);
  digitalWrite(ledPin12, HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  value = analogRead(A0);
  
  //khi rung, value ~ 1023 1024
  value = value / 4;
  Serial.println(value);
  
  if (value >= 255) {
    digitalWrite(ledPin11, LOW);
    digitalWrite(ledPin12, LOW);
    delay(200);

    digitalWrite(ledPin11, HIGH);
    digitalWrite(ledPin12, HIGH);
    delay(200);

    
  }
  
  delay(100);
}
