#include "mechanum_calculator.hpp"

void Mechanum_calculator::set_displacement(double x_arg, double y_arg, double yaw_arg)//입력 단위는 mm 
{
  x = x_arg;
  y = y_arg;
  yaw = yaw_arg;
}

void Mechanum_calculator::set_steps(long int wheel_FR_arg, long int wheel_FL_arg, long int wheel_RR_arg, long int wheel_RL_arg)
{
  wheel_FR = wheel_FR_arg;
  wheel_FL = wheel_FL_arg;
  wheel_RR = wheel_RR_arg;
  wheel_RL = wheel_RL_arg;
}

void Mechanum_calculator::add_steps(long int wheel_FR_arg, long int wheel_FL_arg, long int wheel_RR_arg, long int wheel_RL_arg)
{
  wheel_FR += wheel_FR_arg;
  wheel_FL += wheel_FL_arg;
  wheel_RR += wheel_RR_arg;
  wheel_RL += wheel_RL_arg;
}

void Mechanum_calculator::do_calc_steps()//입력 단위는 mm 
{
  //계산과정에서 스텝의 크기 보다 세밀한 표현은 반올림될 수 있음
  //해당 방식에서는 선회 구현을 생략하여 
  //wheel_size = 80;//mm
  //steps_per_wheel_rev = 48 * 50;// 회전당 스텝수
  //wheel_rev_per_yaw_rev = 1;

  wheel_FR = 0;
  wheel_FL = 0;
  wheel_RR = 0;
  wheel_RL = 0;

  //x 축방향 이동 처리
  wheel_FR += +(long int)((x/wheel_size)*steps_per_wheel_rev);
  wheel_FL += +(long int)((x/wheel_size)*steps_per_wheel_rev);
  wheel_RR += +(long int)((x/wheel_size)*steps_per_wheel_rev);
  wheel_RL += +(long int)((x/wheel_size)*steps_per_wheel_rev);

  //y 축방향 이동 처리
  wheel_FR += -(long int)((y/wheel_size)*steps_per_wheel_rev);
  wheel_FL += +(long int)((y/wheel_size)*steps_per_wheel_rev);
  wheel_RR += +(long int)((y/wheel_size)*steps_per_wheel_rev);
  wheel_RL += -(long int)((y/wheel_size)*steps_per_wheel_rev);


  wheel_FR += +(long int)(((yaw*wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev);
  wheel_FL += -(long int)(((yaw*wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev);
  wheel_RR += +(long int)(((yaw*wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev);
  wheel_RL += -(long int)(((yaw*wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev);
}

//getter를 4개를 만들까?
//배열 포인터로 반환할까?
//콜바이 포인터나 레퍼런스?
//-> 결정 사항 콜바이 레퍼런스 인자가 4개인 getter를 만들자

void Mechanum_calculator::getter_steps_by_ref(long int& wheel_FR_arg, long int& wheel_FL_arg, long int& wheel_RR_arg, long int& wheel_RL_arg)
{
  //call by ref는 양방향 소통이 가능한 방식의 인수 전달
  //call by ref 한 값에다가 대입연산하여 값을 전달하기
  wheel_FR_arg = wheel_FR;
  wheel_FL_arg = wheel_FL;
  wheel_RR_arg = wheel_RR;
  wheel_RL_arg = wheel_RL;
  
}
