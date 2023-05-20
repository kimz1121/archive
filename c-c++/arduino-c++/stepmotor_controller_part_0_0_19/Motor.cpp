#include "Motor.hpp"
#include "Arduino.h"
void Motor::set_gpio(int pin_a_, int pin_b_, int pin_a, int pin_b)
{
  pin_0 = pin_a_;
  pin_1 = pin_b_;
  pin_2 = pin_a;
  pin_3 = pin_b;
  pinMode(pin_0, 1); //OUTPUT으로 설정
  pinMode(pin_1, 1);//OUTPUT으로 설정
  pinMode(pin_2, 1);//OUTPUT으로 설정
  pinMode(pin_3, 1);//OUTPUT으로 설정
}

void Motor::set_state(short state)
{
  motor_state = state;
}

void Motor::set_rest(short state)
{
  rest_mode = state;
}

long int Motor::get_step_on_stack()
{
  return step_stack;
}

long int Motor::set_step_on_stack(int step_stack_arg)
{
  step_stack = step_stack_arg;
}


void Motor::set_pivot()
{
  step_stack_pivot = 0;
}

void Motor::set_pivot(int pivot_val)
{
  step_stack_pivot = pivot_val;
}


long int Motor::get_dispacement_from_pivot()
{
  return step_stack_pivot;
}
void Motor::set_motor_pin_out_bit()
{
  if (rest_mode == 1 && motor_state == -1)//motor_state == -1 // -1 인 겨웅 parking state 이다. 이때 rest_mode 활성화 되어 있다면 모터 출력을 차단함.
  {
    motor_out_bit = 0b0000;
  }
  else
  {
    switch (step_state)//맴버 변수 이용
    {
      case 0:
        motor_out_bit = 0b0100;
        break;
      case 1:
        motor_out_bit = 0b0110;
        break;
      case 2:
        motor_out_bit = 0b0010;
        break;
      case 3:
        motor_out_bit = 0b0011;
        break;
      case 4:
        motor_out_bit = 0b0001;
        break;
      case 5:
        motor_out_bit = 0b1001;
        break;
      case 6:
        motor_out_bit = 0b1000;
        break;
      case 7:
        motor_out_bit = 0b1100;
        break;
    }
  }
}

void Motor::do_motor_pin_write_bit()
{
  int bit_0 = (motor_out_bit >> 0) % 0b10;
  int bit_1 = (motor_out_bit >> 1) % 0b10;
  int bit_2 = (motor_out_bit >> 2) % 0b10;
  int bit_3 = (motor_out_bit >> 3) % 0b10;
  digitalWrite(pin_0, bit_0);
  digitalWrite(pin_1, bit_1);
  digitalWrite(pin_2, bit_2);
  digitalWrite(pin_3, bit_3);
}

bool Motor::set_steps_input(long int steps)
{
  bool retrun = 0;
  if (motor_state == 0) //모터의 상태가 정지 했을 때만 새로운 스텝을 입력 가능
  {
    steps_input = steps;
    retrun = 1;
  }
  else
  {
    retrun = 0;
  }
  return retrun;
}

bool Motor::set_loadSteps()
{
  bool return_val = 0;
  if (step_stack == 0)
  {
    step_stack += steps_input;
    return_val = 1;
  }
  else
  {
    return_val - 1;
  }
  return return_val ;
}

int Motor::get_is_drive_done()
{
  return motor_state;
}

int Motor::ENGINE_drive()//완료 여부 값을 반환 , 0 엔진 작동중, 1 엔진 작동 완료, -1 작동오류
{
  if (step_stack != 0)
  {
    motor_state = 1;//모터 작동에 대한 상태를 반환하는 일종의 안전장치
    // rest_mode = 0;
    if (step_stack > 0)//스텝 입력이 있는 경우
    {

      step_state += +1;//스텝 상태 연산
      if (step_state > 7)
      {
        step_state = 0;
      }
      set_motor_pin_out_bit();//모터 출력 비트 준비
      do_motor_pin_write_bit();//준비된 비트를 모터에 출력
      step_stack -= + 1;
      step_stack_pivot += + 1;
    }
    if (step_stack < 0)
    {
      step_state += -1;//스텝 상태 연산
      if (step_state < 0)
      {
        step_state = 7;
      }
      set_motor_pin_out_bit();//모터 출력 비트 준비
      do_motor_pin_write_bit();//준비된 비트를 모터에 출력
      step_stack -= - 1;//step_stack은 step을 비워나감
      step_stack_pivot += - 1;//피step_stack_pivot은 step을 쌓아서 측정함.
    }
  }
  else
  {
    motor_state = 0;
    // rest_mode = 1;
    set_motor_pin_out_bit();//모터 출력 비트 준비//rest_mode 돌입을 위한 구동
    do_motor_pin_write_bit();//준비된 비트를 모터에 출력
  }
  return motor_state;
}
