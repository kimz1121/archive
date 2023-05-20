#include <Servo.h>
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MotorPin 6
Servo servo;
int spd = 80;
int seta = 0;

void setup () {
Serial.begin(9600);
delay(3000);
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

//servo.attach(8);



}

void loop () {
  if (spd >= 170) 
  {
  spd = 100;
  }
  if (seta >= 230) 
  {
  seta = 100;
  }


  seta = seta+3;
  spd = spd+5;
  
  analogWrite(9, spd);
  analogWrite(6, seta);
  
  
  Serial.print("Motor speed = "); 
  Serial.print(spd);
  Serial.print("servo degree = ");
  Serial.println(seta);
  
  delay(500);
}
