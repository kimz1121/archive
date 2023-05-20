#include <Servo.h>
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MotorPin 9
Servo motor;
int spd = 80;


void setup () {
Serial.begin(9600);
delay(5000);
/*
motor.attach(MotorPin);
motor.writeMicroseconds(MAX_SIGNAL); //2000
delay(50);
motor.writeMicroseconds(MIN_SIGNAL); //1000
delay(2000);
*/
analogWrite(9, 250);
delay(1000);
analogWrite(9, 0);
delay(1000);

}

void loop () {
  if (spd >= 230) 
  {
  spd = 0;
  }

  
  spd = spd+5;
  
  analogWrite(9,spd);
  Serial.print("Motor speed = "); Serial.println(spd);
  delay(250);
}
