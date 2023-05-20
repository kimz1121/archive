const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, HIGH);
}
void loop() 
{
  Serial.println("Loop....... ");
  digitalWrite(ledPin, state);
  delay(100);
}
void blink() 
{
  Serial.println("It is in isr");
  state = !state;
}
