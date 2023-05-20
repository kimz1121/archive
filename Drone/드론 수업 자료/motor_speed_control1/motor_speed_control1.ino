#define MIN 40


int deg = 0;
int spd = 40;
int motorpin = 9; //PWM pin only. pin 3,5,6,9,10,11
void setup() {
Serial.begin(9600);
analogWrite(motorpin, MIN); //모터 초기화
}

void loop() {
  spd = map(deg , 180,0, 255, 0);
if (deg < 180) 
deg = deg + 1;
  if (deg >= 180) 
  {
  analogWrite(motorpin,spd); // spd: 8비트이므로 0 ~ 255
  delay(500);
  deg = 40;
  }
analogWrite(motorpin,spd);
Serial.print("spd = ");
Serial.println(spd, deg);
delay(200);
}
