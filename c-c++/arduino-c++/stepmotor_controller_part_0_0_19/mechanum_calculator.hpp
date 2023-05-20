#ifndef MECHANUM_CALCULATOR_H
#define MECHANUM_CALCULATOR_H

class Mechanum_calculator
{
private:
  const int wheel_size = 80;//mm
  const int steps_per_wheel_rev = 48 * 50;// 회전당 스텝수
  const double wheel_travle_per_yaw_rev = 8*80;
  
  double x = 0;
  double y = 0;
  double yaw = 0;

  long int wheel_FR = 0;
  long int wheel_FL = 0;
  long int wheel_RR = 0;
  long int wheel_RL = 0;

public:
  void set_displacement(double x_arg, double y_arg, double yaw_arg);
  void set_steps(long int wheel_FR_arg, long int wheel_FL_arg, long int wheel_RR_arg, long int wheel_RL_arg);
  void add_steps(long int wheel_FR_arg, long int wheel_FL_arg, long int wheel_RR_arg, long int wheel_RL_arg);
  void do_calc_steps();
  //getter를 4개를 만들까?
  //배열 포인터로 반환할까?
  //콜바이 포인터나 레퍼런스?
  //-> 결정 사항 콜바이 레퍼런스 인자가 4개인 getter를 만들자
  
  void getter_steps_by_ref(long int& wheel_FR_arg, long int& wheel_FL_arg, long int& wheel_RR_arg, long int& wheel_RL_arg);
  
};



#endif
