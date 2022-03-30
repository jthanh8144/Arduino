int ledPin = 13;

void tatled() {
  digitalWrite(ledPin, LOW);  // tắt đèn led
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(2, INPUT_PULLUP);         // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
  attachInterrupt(0, tatled, LOW);  // gọi hàm tatled liên tục khi còn nhấn nút
}

void loop() {
  digitalWrite(ledPin, HIGH);  // bật đèn
}