#include <Servo.h>
#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MotorPin 9
Servo motor;
int DELAY = 800;


void setup () {
  Serial.begin(9600);
  delay(2000);
  motor.writeMicroseconds(0);
  delay(2000);
  motor.attach(MotorPin);
  motor.writeMicroseconds(MAX_SIGNAL); //2000
  delay(1000);
  motor.writeMicroseconds(MIN_SIGNAL); //1000
  delay(2000);



}

void loop () {
  if(Serial.available()>0)
  {
    int DELAY = Serial.parseInt();
    if(DELAY> 0)
      {
        motor.writeMicroseconds(DELAY);
      }
  }
}
