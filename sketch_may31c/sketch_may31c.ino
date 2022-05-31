#define PIN_DO 2
volatile unsigned int pulses;
float rpm;
unsigned long timeOld;
#define HOLES_DISC 1

void counter()
{
  pulses++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_DO, INPUT);
  pulses = 0;
  timeOld = 0;
  attachInterrupt(digitalPinToInterrupt(PIN_DO), counter, FALLING);
}

void loop()
{
  if (millis() - timeOld >= 1000)
  {
    detachInterrupt(digitalPinToInterrupt(PIN_DO));
    rpm = (pulses * 60) / (HOLES_DISC);
    Serial.println(rpm);
    
    timeOld = millis();
    pulses = 0;
    attachInterrupt(digitalPinToInterrupt(PIN_DO), counter, FALLING);  
  }
}