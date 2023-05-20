#include "Motor.hpp"

#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

/*
 * 모터 매니저 역할
 *  각도 관리
 *    스텝 단위로 관리되는 모터의 시스템의 스텝을 관리하여 제어의 차원을 각도 제어 수준으로 변환한다.
 *
 *  기능 1
 *    입력 한 각도의 크기 중에 대해 실제 반영된 각도의 양을 반환...>
 *      ...>아두이노 차원에서는 애초에 스텝단위로 이동을 입력 받도록 한다면....장점 : 통신시 이동각도에 대한 정밀도 유지 가능, 단점 : 실시간 통신량이 매우 크게 증가함. 혹은 정확해야함. 스텝이라는 아주 작은 단위로 세분화되어서 이동이 구현되기 때문
 *      ->일단 스텝단위로 이동을 입력 받는 것을 가정함. 각도 제어의 중요성이 더욱 크다고 판단 하였기 때문
 *  기능 2
 *    모터의 ENGINE을 Supervisor 까지 전달
 *
 *  기능 3
 *    모터의 엔진을 관리하는 일련의 과정을 관리
 *    준비
 *    구동
 *    휴식 모드를 자동으로 제어
 *    즉 매니저가 전달하는 ENGINE은
 *    종료 기준이 MOTOR의 ENGINE 과는 조금 다르다.
 *
 * 현상 구현 대상
 * ->
 *
 */
class Motor_manager //아두이노에서의 모터 매니져는 파이썬에서의 모터 매니져와 조금 다른 개념으로 고려 하여야 할 듯, 왜냐하면 모터의 구동만을 담당하고, 모터의 각도 정보같은 것들까지는 반환할 필요가 없을 듯함. 혹은 완료시 각도 변화정보를 따로 반환할 방법이 있는지 생각해보기
{
  //파이썬에서 각도 정보를 반환하면 자동으로 즉시 처리하여 로봇의 로컬라이제이션을 처리하면 좋겠지만,,,, 파이썬 내부적으로 로컬라이제이션을 별도 수행하는 것이 좋을 듯함.
  //
public:
  Motor motor;
  //프로세스 상태 관리
  int process_phase = 0;
  //프로세스 자원 관리
  bool steps_state = 0; //스텝 입력 대기 여부
  bool motor_state = 0; //모터 주행 완료 여부
  // bool engine_management_state = 0;//매니져 기준 모터 구동 완료 여부
  // 0 대기중
  // 1 동작중
  // 2 동작 완료

  //자원 변수
  long int steps = 0;
  //자원 조작 합수
  bool set_step_input(long int step_input);

  void set_measure_pivot();
  long int get_dispacement_from_pivot();

  void set_process_stop_forced();

  // void process_stop();

  // void process_stop_forced();

  int ENGINE_manager();
};

#endif
