#include "Motor_manager.hpp"
#include "Motor.hpp"
// 매니저 이후 단계에서는 스텝을 넣고 돌리는 2가지로 구동을 끝내기

bool Motor_manager::set_step_input(long int step_input)
{
  bool state_success = 0;
  if (steps_state == 0)
  {
    if (step_input != 0)
    {
      //이미 대기 중인 스텝 자원이 있는 경우에는 스텝을 새롭게 입력 받지 않음.
      //입력되는 스텝이 없는 스텝이어서도 안됨.
      steps = step_input;
      steps_state = 1;
      state_success = 1;
    }
    else
    {
      //입력되는 스텝이 없는 스텝인 경우는 기존의 스텝을 삭제하는 기능으로 써 별도 처리
      steps = 0;
      state_success = 1;
    }
  }
  else
  {
    state_success = 0;
  }
  return state_success;
}

void Motor_manager::set_measure_pivot()
{
  motor.set_pivot();
}


long int Motor_manager::get_dispacement_from_pivot()
{
  return motor.get_dispacement_from_pivot();
}


void Motor_manager::set_process_stop_forced()
{
  steps_state = 0;
  set_step_input(0);//step_stack을 비워주지 못해 s가 다시 입력되면 주행이 이어서 진행됨.
  motor.set_step_on_stack(0);
  ENGINE_manager();
  ENGINE_manager();
}

int Motor_manager::ENGINE_manager()
{
  //동작에 필요한 자원들을 점검하여 충분하면 모터를 park(-1) 상태에서 ready(0) 상태로 전환 또는 ready(0) 를 유지
  //동작이 시작 되었다면 ready(0) 상태를 drive(1) 상태로 전환
  //동작이 완료 되었으면 drive(1) 상태를 ready(0) 상태로 전환

  //process_phase 0 모터 구동 안함, 자원 비어짐
  //process_phase 1 모터 구동 안함, 자원 준비 완료
  //process_phase 2 모터 구동 , 자원 비어짐
  //process_phase 3 모터 구동 , 자원 준비 완료 // 해당 상태는 아직 모터가 구동 중인데 목표를 입력하려고 한경우 이다. 해당 경우는 애초에 일어나 지 않도록 process_phase2 에서 방어한다.
  // do_step_source_control();
  //phase 관리
  switch (process_phase)
  {
    case 0://stop
      if (steps_state == 1)
      {
        process_phase = 1;
      }
      break;
    case 1://Stanby
      motor.set_state(0);//parking 된경우 parking을 해제함.
      motor.set_steps_input(steps);
      motor.set_loadSteps();//준비된 스텝들을 모터에 적제.
      motor.set_state(1);//적재후 모터를 가동 가능한 상태로 준비.
      motor.set_rest(0);//휴식 모드 해제 //모터에 전류 인가.
      process_phase = 2;
      break;
    case 2://working
      motor_state = motor.get_is_drive_done();
      if (motor_state == 1) //모터의 상태가 주행중이라면
      {
        motor.ENGINE_drive();//모터를 구동
      }
      else if (motor_state == 0) //구동이 완료 되었을 시 마무리 단계로 넘어감.
      {
        process_phase = 3;
      }
      break;
    case 3://finish
      motor.set_rest(1);//휴식모드 //모터에 전류 차단.
      motor.ENGINE_drive();
      steps_state = 0;//모터 구동이 완전히 완료 된 이후 새로운 스텝을 받을 상태가 됨.
      process_phase = 0;
      break;
    default:
      break;
  }
  return process_phase;
}
