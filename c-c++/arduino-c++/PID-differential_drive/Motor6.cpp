#include <MsTimer2.h>

#define encoderPinL_A 50
#define encoderPinL_B 52
#define encoderPinR_A 51
#define encoderPinR_B 53

#define IN1 30
#define IN2 32
#define ENA 12
#define IN3 34
#define IN4 36
#define ENB 11

volatile boolean ENC_R_A_Recent_val = 0;
volatile boolean ENC_R_B_Recent_val = 0;
volatile boolean ENC_L_A_Recent_val = 0;
volatile boolean ENC_L_B_Recent_val = 0;

volatile long int encoder_R_shift = 0;//R엔코더 값 저장하는 변수
volatile long int encoder_L_shift = 0;//L엔코더 값 저장하는 변수

float Pv = 100;
float Iv = 0.1;
float Dv = 20;


float previous_err_L;
float previous_err_R;

int number = 0;
int reperence_L = 0;
int reperence_R = 0;
int PID_val_L;
int PID_val_R;
double estep_readL = 0;
double estep_readR = 0;
double Input_L = 0;
double Input_R = 0;


void encoderL() {
  encoder_L_shift++;
}
void encoderR() {
  encoder_R_shift++;
}

void encoder_read() {
  estep_readL = encoder_L_shift;
  encoder_L_shift = 0;
  estep_readR = encoder_R_shift;
  encoder_R_shift = 0;
}


void pid_control_L()
{
  float err_P;
  float err_I;
  float err_D;
  err_P = estep_readL - reperence_L;
  err_I += err_P;
  err_D = previous_err_L - err_P;
  previous_err_L = err_P;
  PID_val_L = ((err_P * Pv) + (err_I * Iv) + (err_D * Dv));
  //  PID_val_L = ((err_P * Pv) + (err_D * Dv));
  if (PID_val_L >= 255) PID_val_L = 255;
  if (PID_val_L <= -255) PID_val_L = -255;
  motor_driver_L(PID_val_L);
}

void pid_control_R()
{
  float err_P;
  float err_I;
  float err_D;
  err_P = estep_readR - reperence_R;
  err_I += err_P;
  err_D = previous_err_R - err_P;
  previous_err_R = err_P;
  PID_val_R = ((err_P * Pv) + (err_I * Iv) + (err_D * Dv));
  //  PID_val_R = ((err_P * Pv) + (err_D * Dv));
  if (PID_val_R >= 255) PID_val_R = 255;
  if (PID_val_R <= -255) PID_val_R = -255;
  motor_driver_R(PID_val_R);
}

void motor_driver_L(int PID_val_L)
{
  boolean MOTOR_direction = 0;
  int OUTPUT_PWM = 0;
  boolean input_A_0;
  boolean input_A_1;
  if (PID_val_R > 0)
  {
    MOTOR_direction = 1;
  }
  else
  {
    MOTOR_direction = 0;
  }

  if (abs(PID_val_L)  >= 255)
  {
    OUTPUT_PWM = 255;
  }
  else
  {
    OUTPUT_PWM = abs(PID_val_L);
  }

  switch (MOTOR_direction)
  {
    case 1:
      input_A_0 = 1;
      input_A_1 = 0;
      break;
    case 0:
      input_A_0 = 0;
      input_A_1 = 1;
      break;
  }

  digitalWrite(IN2, input_A_0);
  digitalWrite(IN1, input_A_1);
  analogWrite(ENA, OUTPUT_PWM);


}



void motor_driver_R(int PID_val_R)
{
  boolean MOTOR_direction = 0;
  int OUTPUT_PWM = 0;
  boolean input_A_0;
  boolean input_A_1;
  if (PID_val_R > 0)
  {
    MOTOR_direction = 1;
  }
  else
  {
    MOTOR_direction = 0;
  }

  if (abs(PID_val_R)  >= 255)
  {
    OUTPUT_PWM = 255;
  }
  else
  {
    OUTPUT_PWM = abs(PID_val_R);
  }

  switch (MOTOR_direction)
  {
    case 1:
      input_A_0 = 1;
      input_A_1 = 0;
      break;
    case 0:
      input_A_0 = 0;
      input_A_1 = 1;
      break;
  }

  digitalWrite(IN4, input_A_0);
  digitalWrite(IN3, input_A_1);
  analogWrite(ENB, OUTPUT_PWM);


}

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //  pinMode(encoderOpinA, INPUT);
  //  pinMode(encoderOpinB, INPUT);
  pinMode(encoderPinR_A, INPUT);
  pinMode(encoderPinR_B, INPUT);
  pinMode(encoderPinL_A, INPUT);
  pinMode(encoderPinL_B, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  MsTimer2::set(20, t_intrrupt);
  MsTimer2::start();
  //  attachInterrupt(0, encoderOpinL_A, FALLING);
  //  attachInterrupt(0, encoderOpinL_B, FALLING);
  //  attachInterrupt(1, encoderOpinR_A, FALLING);
  //  attachInterrupt(1, encoderOpinR_B, FALLING);



  Serial.begin(9600);

  set_and_init_interrupt();


}

void loop()
{
  if (number == 2) {
    Serial.println("number == 2");
    //    analogWrite(ENA, 255);
    //    digitalWrite(IN1, HIGH);
    //    digitalWrite(IN2, LOW);
    //    analogWrite(ENB, 255);
    //    digitalWrite(IN3, HIGH);
    //    digitalWrite(IN4, LOW);
    reperence_L = 100;
    reperence_R = 100;
  }
  else if (number == 1) {
    Serial.println("number == 1");
    //    analogWrite(ENA, 150);
    //    digitalWrite(IN1, HIGH);
    //    digitalWrite(IN2, LOW);
    //    analogWrite(ENB, 50);
    //    digitalWrite(IN3, HIGH);
    //    digitalWrite(IN4, LOW);
    reperence_L = 100;
    reperence_R = 150;
  }
  else if (number == 3) {
    Serial.println("number == 3");
    //    analogWrite(ENA, 50);
    //    digitalWrite(IN1, HIGH);
    //    digitalWrite(IN2, LOW);
    //    analogWrite(ENB, 150);
    //    digitalWrite(IN3, HIGH);
    //    digitalWrite(IN4, LOW);
    reperence_L = 150;
    reperence_R = 100;
  }
  else {
    //    analogWrite(ENA, 0);
    //    analogWrite(ENB, 0);
    reperence_L = 0;
    reperence_R = 0;
  }

  Input_L = encoder_L_shift * 360 / 3840;
  Input_R = encoder_R_shift * 360 / 3840;
  if (Serial.available() > 1)
  {
    number = Serial.parseInt();
    Serial.println("parseInt");
    Serial.println(number);
    Serial.flush();
  }
  Serial.println("º");

  Serial.print("encoderR : ");
  Serial.print(encoder_R_shift);
  Serial.print("  |  ");
  Serial.print("encoderL : ");
  Serial.println(encoder_L_shift);
}

void set_and_init_interrupt()
{
  PCICR = (1 << PCIE0);
  PCMSK0 = (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1) | (1 << PCINT0);
  boolean ENC_R_A_Recent_val = digitalRead(encoderPinR_A);
  boolean ENC_R_B_Recent_val = digitalRead(encoderPinR_B);
  boolean ENC_L_A_Recent_val = digitalRead(encoderPinL_A);
  boolean ENC_L_B_Recent_val = digitalRead(encoderPinL_B);
}


ISR(PCINT0_vect)
{
  boolean ENC_R_A = digitalRead(encoderPinR_A);
  boolean ENC_R_B = digitalRead(encoderPinR_B);
  boolean ENC_L_A = digitalRead(encoderPinL_A);
  boolean ENC_L_B = digitalRead(encoderPinL_B);
  //ENC_A_Recent_val;
  //ENC_A_Recent_val;


  //encoder_R------------------------
  if (ENC_R_A ^ ENC_R_A_Recent_val == 1)
  {
    if (ENC_R_B == 1)
    {
      if (ENC_R_A == 1)
      {
        encoder_R_shift++;
      }
      else
      {
        encoder_R_shift--;
      }
    }
    else
    {
      if (ENC_R_A == 0)
      {
        encoder_R_shift++;
      }
      else
      {
        encoder_R_shift--;
      }
    }
  }
  if (ENC_R_B ^ ENC_R_B_Recent_val == 1)
  {
    if (ENC_R_A == 1)
    {
      if (ENC_R_B == 1)
      {
        encoder_R_shift--;
      }
      else
      {
        encoder_R_shift++;
      }
    }
    else
    {
      if (ENC_R_B == 0)
      {
        encoder_R_shift--;
      }
      else
      {
        encoder_R_shift++;
      }
    }
  }
  //<encoder_L>------------------------
  if (ENC_L_A ^ ENC_L_A_Recent_val == 1)
  {
    if (ENC_L_B == 1)
    {
      if (ENC_L_A == 1)
      {
        encoder_L_shift++;
      }
      else
      {
        encoder_L_shift--;
      }
    }
    else
    {
      if (ENC_L_A == 0)
      {
        encoder_L_shift++;
      }
      else
      {
        encoder_L_shift--;
      }
    }
  }
  if (ENC_L_B ^ ENC_L_B_Recent_val == 1)
  {
    if (ENC_L_A == 1)
    {
      if (ENC_L_B == 1)
      {
        encoder_L_shift--;
      }
      else
      {
        encoder_L_shift++;
      }
    }
    else
    {
      if (ENC_L_B == 0)
      {
        encoder_L_shift--;
      }
      else
      {
        encoder_L_shift++;
      }
    }
  }
  ENC_R_A_Recent_val = ENC_R_A;
  ENC_R_B_Recent_val = ENC_R_B;
  ENC_L_A_Recent_val = ENC_L_A;
  ENC_L_B_Recent_val = ENC_L_B;
}

void t_intrrupt()
{
  encoder_read();
  pid_control_L();
  pid_control_R();
}
