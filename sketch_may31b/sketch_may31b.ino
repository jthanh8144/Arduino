

// int ENA = D7;
// int IN1 = D1;
// int IN2 = D2;
int ENA = D6;
int IN1 = D3;
int IN2 = D4;
int sensor = D7;

volatile unsigned int pulses;
float rpm;
unsigned long timeOld;
int holesDisc = 1;

void ICACHE_RAM_ATTR counter();

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  pinMode(sensor, INPUT);
  pulses = 0;
  timeOld = 0;
  attachInterrupt(digitalPinToInterrupt(sensor), counter, FALLING);
}

void loop() {
  analogWrite(ENA, 10);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  if (millis() - timeOld >= 1000)
  {
    detachInterrupt(digitalPinToInterrupt(sensor));
    rpm = (pulses * 60) / (holesDisc);
    Serial.println(rpm);
    
    timeOld = millis();
    pulses = 0;
    attachInterrupt(digitalPinToInterrupt(sensor), counter, FALLING);  
  }
}

void ICACHE_RAM_ATTR counter() {
  pulses++;
}
