#include <Servo.h>
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN4 4
Servo motor;
unsigned long counter_1, current_count;
byte last_CH1_state = 0;
int Ch1;                     // channel 1 of the receiver and pin D8 of arduino


void setup () {                 //모터 calibration

  Serial.begin(250000);
  motor.attach(MOTOR_PIN4);
  motor.writeMicroseconds(MAX_SIGNAL);
  delay(1000);
  motor.writeMicroseconds(MIN_SIGNAL);

  PCICR |= (1 << PCIE0);                // set PCIE0 to enable PCMSK0 scan
  PCMSK1 |= (10 << PCINT10);                // set PCINT0=(digital input 8) to trigger an interrupt on state change
}
void loop() {
  /* Ok, so in the loop the only thing that we do in this example is to print
    the received values on the Serial monitor. The PWM values are read in the
    ISR below. */


  Serial.print("Ch1: ");
  Serial.println(Ch1);
  if(Ch1 >= 1500)
  {
    digitalWrite(13, 1);
  }
  else
  {
  digitalWrite(13, 0);
  }
}



ISR(PCINT1_vect) {                  //Channel 1= =============
  current_count = micros();
  if (PINC & B00000100)
  { //We verify if pin 8 is HIGH???

    if (last_CH1_state == 0)
    { //If state was 0, we have a state change...
      last_CH1_state = 1;           //Store the current state into the last state
      counter_1 = current_count;          //Set counter_1 to current value.
    }
  }

  else if (last_CH1_state == 1)
  { //If pin 8 is LOW and the last state was HIGH then we have a state change
    last_CH1_state = 0;             //Store the current state into the last state
    Ch1 = current_count - counter_1;
  }
}
