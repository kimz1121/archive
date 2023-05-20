#include <PinChangeInt.h>



const byte ledPin = 13;
const byte interruptPin = A0;
volatile byte state = LOW;
void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  PCintPort::attachInterrupt(interruptPin, blink, HIGH);// Pin Change Interuppt Port
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
