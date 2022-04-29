int ledPin = 7;

void tatled() {
  digitalWrite(ledPin, HIGH);  // tắt đèn led
  Serial.println("btn");
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(2, INPUT_PULLUP);         // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
  attachInterrupt(0, tatled, RISING);  // gọi hàm tatled liên tục khi còn nhấn nút
  Serial.println("setup");
}

void loop() {
  // Serial.println("loop");
  digitalWrite(ledPin, LOW);  // bật đèn
}