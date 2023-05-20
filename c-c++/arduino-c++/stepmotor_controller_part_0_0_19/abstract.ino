  /*
 * 개요
 * 
 * 목표 1 모터를 원하는 각도 만큼 돌리킄 프로그램을 제작
 *  1. 모터 슈퍼바이져에 각 모터 별로 원하는 회전 스텝을 입력하기
 *  2. 모터 매니져가 각 모터로 스텝을 분배하기
 *  3. 각 모터가 받은 스텝을 구동하기
 *  4. 구동할때는 Engine 을 구현하여 동시에 4개의 모터를 돌릴 수 있도록 만들기
 *
 *
 *
 *
 * 0.0.2 버전 설명
 * 
 * 개발 언어
 *      C++
 * 
 * 외부 라이브러리
 *      Arduino.h
 * 
 * 개요
 *      
 *
 * 목표 모터 매니저를 작성하여 모터의 구동 과정을 추상화
 *
 * 0.0.4 버전 
 *  통신을 통해 구동 시스템을 제어할 수 있도록 설계
 *  제어 방식은 X Y YAW로 제어 
 * 
 * 0.0.5 버전
 *  작동 결과를 통신으로 피드백 하기
 *    -피드백 내용, 작동 상태(구동중 완료중)
 *    -모터의 스텝 회전
 * 
 * 
 * 0.0.6 버전
 *  작동의 결과를 반환하는 피드백을 로봇이 이해하기 쉬운 형태로 정리
 *  ',' 문자로 구분된 값을 주로 이용 
 * 
 * 0.0.7 버전 
 *  중간 중단 기능을 추가->아직 못함.......0.0.7 버전에서 구현하지 못함.
 *  중단 피드백 신호를 다른 MCU가 이해하기 적합한 형태로 변형
 *  
 *  약깐의 Motor_supervisor 기능 구현 추가
 *    헤더파일 선언 추가 수준 CPP 구현은 아직 하지 않음
 *    
 *  + 딜레이용 loop의 오류를 수정함 
 *    -time_before_control 변수가 micros() 가 아닌  millis() 함수에 의해 초기화 되고 있었음
 *  최적화 되는 것을 방지하기 위해 
 *  
 * 0.0.8 버전 
 *  헤더파일들의 inclusion guard를 만들때의 오류들을 수정하여 보완함.
 *  
 * 0.0.9 버전
 *  헤더파일 Motor의 객체 구현 파일은 Motor.cpp 코드에서  
 *  ENGINE_drive() 함수가 음수의 크기를 갖는 스텝입력을 제대로 처리하지 못하는 현상을 해결함.
 *    -원인 지난 코드 수정에서 부호를 틀렸던 것이 문제
 *    
 *    step_stack -= + 1;//step_stack은 step을 비워나감 
 *    ->step_stack -= - 1;//step_stack은 step을 비워나감
 * 
 * 0.0.11 버전
 *  yaw kinematics 계산 방법에 대한 오류를 수정
 *    wheel_FR += +(long int)(((yaw/wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev)
 *    ->wheel_FR += +(long int)(((yaw*wheel_travle_per_yaw_rev)/wheel_size)*steps_per_wheel_rev)
 * 
 * 0.0.12 버전
 *  mechanum calculator 의 구조를 변경함.
 *    -set 과 clac 의 분리 
 *    -input step과 input displace 의 변경.
 * 
 * 0.0.13 버전
 *   통신의 방식을 x, y, yaw 값으로 받는 것이 아닌 스텝 단위로 바꾸도록 수정함
 * 
 * 0.0.14 버전 
 *  통신의 응답이 너무 느린 것을 개선하기위해
 *  void setup영역에 Serial.setTimeout(10); 코드를 추가함.
 *  
 *  0.0.15 버전
 *    작동 중 중단할 필요성을 느낌 
 *    
 *  0.0.16 버전
 *    s 를 처음 입력할 때는 중단이 잘 적용이 되지만
 *    s 를 2번 째 이후로 연달아 입력하는 경우 정지가 해제되며 미쳐다 주행하지 못했던 구간을 주행한다.
 *    s 를 통해 주행을 멈추게 되었을 때 step_stack을 강제로 비워줄 필요가 있다.
 * 
 * 0.0.17
 *    s를 입력 받은 후 중단이 잘 되며 
 *    s를 2번 입력 받아도 문제가 없지만
 *    s를 입력 받은 바로 다음의 주행 명령이 무시되는 현상이 0.0.16 버전에 발견되어 수정하고자 함.
 * 
 * 
 * 
 * 
 */