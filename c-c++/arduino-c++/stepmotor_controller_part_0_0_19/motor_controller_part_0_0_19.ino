#include "Motor.hpp"
#include "Motor_manager.hpp"
//#include "Motor_supervisor.hpp"
#include "mechanum_calculator.hpp"

//<전역변수 사작>----------------------

//모터 객체 전역 변수
Motor_manager Manager0;
Motor_manager Manager1;
Motor_manager Manager2;
Motor_manager Manager3;

//모터 ENGINE 상태 변수

int process_phase[4] = { -1, -1, -1, -1};

//작업 상태 관리 변수
bool state_all_done = 0;
bool forced_stop = 0;
// process_phase 4개의 페이즈가 모두 0인지 확인하는 함수가 있어야 할 듯
//함수 이름은 get_is_all_done


//모터 기구학 계산기
Mechanum_calculator Kinematic_calculator;

//이동 거리 입력 변수
int x = 0;
int y = 0;
double yaw = 0;


// 스텝수 변수
long int step_FR = 0;
long int step_FL = 0;
long int step_RR = 0;
long int step_RL = 0;

//스텝수 변화량 변수
long int step_FR_displacement = 0;
long int step_FL_displacement = 0;
long int step_RR_displacement = 0;
long int step_RL_displacement = 0;

//리프트 구동 제어 변수
int charge_state_val = 0;

int pin_a = 26;
int pin_b = 28;

int lift_drive_time = 5000;//ms 단위 리프트 구동 시간

//통신 전역 변수
String string_input;
volatile bool serial_lock = 0;

String data_0 = "";
String data_1 = "";
String data_2 = "";
String data_3 = "";
String data_4 = "";

char mode_val = 0;
char command = 0;

//loop time 측정용 변수
volatile unsigned long int time_before_control = 0;
volatile unsigned long int time_before_feedback = 0;
volatile unsigned long int time_before_liftloop = 0;

//<전역변수 종료>----------------------
void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10);
  Manager0.motor.set_gpio(2, 3, 4, 5);//FR
  Manager1.motor.set_gpio(6, 7, 8, 9);//FL
  Manager2.motor.set_gpio(10, 11, 12, 13);//RR
  Manager3.motor.set_gpio(14, 15, 16, 17);//LL
}

void loop()
{
  //  Serial.println("Wait for input");
  while (serial_lock == 0)//UART 통신을 통한 입력이 들어올 때까지 무한 대기
  {
    if ((micros() - time_before_feedback) > 1000) //delay를 쓰는 것 은다른 이벤트 처리를 방해할 우려가 큼.
    {
      time_before_feedback = micros();
      //      Serial.println("-----");
    }
    if (Serial.available() > 0)
    {
      string_input = Serial.readString();

      int index0 = 0;
      int index1 = 0;
      int index2 = 0;
      int index3 = 0;

      index0 = string_input.indexOf(",", 0);
      index1 = string_input.indexOf(",", index0 + 1);
      index2 = string_input.indexOf(",", index1 + 1);
      index3 = string_input.indexOf(",", index2 + 1);

      data_0 = string_input.substring(0, index0);
      data_1 = string_input.substring(index0 + 1, index1);
      data_2 = string_input.substring(index1 + 1, index2);
      data_3 = string_input.substring(index2 + 1, index3);
      data_4 = string_input.substring(index3 + 1);

      mode_val = data_0[0];

      Serial.println("-recive");

      if (mode_val == 'w' || mode_val == 'W')
      {
        String string_input_step_FR = data_1;
        String string_input_step_FL = data_2;
        String string_input_step_RR = data_3;
        String string_input_step_RL = data_4;

        step_FR = string_input_step_FR.toInt();
        step_FL = string_input_step_FL.toInt();
        step_RR = string_input_step_RR.toInt();
        step_RL = string_input_step_RL.toInt();
        //      Serial.println(x);
        //      Serial.println(y);
        //      Serial.println(yaw);
        serial_lock = 1;
      }
      if (mode_val == 'l' || mode_val == 'L')
      {
        command = data_1[0];
        if (command == 'u' || command == 'U')
        {
          charge_state_val = 1;
        }
        if (command == 'd' || command == 'D')
        {
          charge_state_val = 5;
        }
        if (command == 's' || command == 'S')
        {
          charge_state_val = 0;
        }
        serial_lock = 1;
      }
    }
  }
  serial_lock = 0;

  // Kinematic_calculator.set_displacement(x, y, yaw);
  // Kinematic_calculator.do_calc_steps();
  // Kinematic_calculator.getter_steps_by_ref(step_FR, step_FL, step_RR, step_RL);

  if (mode_val == 'w' || mode_val == 'W')
  {
    wheel_exeacute();
  }

  if (mode_val == 'l' || mode_val == 'L')
  {
    lift_exeacute();
  }
}

void lift_exeacute()
{
  while (1)
  {
    //    Serial.println("333333");
    //    Serial.println(charge_state_val);

    if (Serial.available() > 0)
    {
      string_input = Serial.readString();
      Serial.println(string_input);
      if (string_input[0] == 's' || string_input[0] == 'S')
      {
        charge_state_val = 0;
      }
      if (string_input[0] == 'd' || string_input[0] == 'D')
      {
        charge_state_val = 5;
      }
    }

    if (charge_state_val == 0) //mode : start lift up
    {
      digitalWrite(pin_a, LOW);
      digitalWrite(pin_b, LOW);
      Serial.println("-N");
      break;
    }
    if (charge_state_val == 1) //mode : start lift up
    {
      time_before_liftloop = millis();//write start time of lift move to up.
      charge_state_val = 2;
      Serial.println("-up");
    }
    if (charge_state_val == 2)
    {
      if (millis() - time_before_liftloop > lift_drive_time)
      {
        charge_state_val = 3;
      }
      digitalWrite(pin_a, LOW);
      digitalWrite(pin_b, HIGH);
    }
    if (charge_state_val == 3)
    {
      time_before_liftloop = millis();//write start time of lift stop.
      charge_state_val = 4;
      Serial.println("-stop");
    }
    if (charge_state_val == 4)
    {
      if (millis() - time_before_liftloop > lift_drive_time)
      {
        charge_state_val = 5;
      }
      digitalWrite(pin_a, LOW);
      digitalWrite(pin_b, LOW);
    }
    if (charge_state_val == 5)
    {
      time_before_liftloop = millis();//write start time of lift move to down.
      charge_state_val = 6;
      Serial.println("-down");
    }
    if (charge_state_val == 6)
    {
      if (millis() - time_before_liftloop > lift_drive_time)
      {
        charge_state_val = 0;
      }
      digitalWrite(pin_a, HIGH);
      digitalWrite(pin_b, LOW);
    }
  }
}

void wheel_exeacute()
{
  Serial.print("-s : ");
  Serial.print(step_FR);
  Serial.print(", ");
  Serial.print(step_FL);
  Serial.print(", ");
  Serial.print(step_RR);
  Serial.print(", ");
  Serial.print(step_RL);
  Serial.println(" x ");

  forced_stop = 0;
  Manager0.set_step_input(step_FR);
  Manager1.set_step_input(-step_FL);
  Manager2.set_step_input(step_RR);
  Manager3.set_step_input(-step_RL);

  Manager0.set_measure_pivot();
  Manager1.set_measure_pivot();
  Manager2.set_measure_pivot();
  Manager3.set_measure_pivot();

  time_before_control = micros();
  time_before_feedback = micros();
  while (1)
  {
    if (Serial.available() > 0)//s 로 시작하는 통신 입력시 작동이 중단.
    {
      string_input = Serial.readString();
      if (string_input.charAt(0) == 's')
      {
        forced_stop = 1;
        Manager0.set_process_stop_forced();
        Manager1.set_process_stop_forced();
        Manager2.set_process_stop_forced();
        Manager3.set_process_stop_forced();
      }
    }
    process_phase[0] = Manager0.ENGINE_manager();
    process_phase[1] = Manager1.ENGINE_manager();
    process_phase[2] = Manager2.ENGINE_manager();
    process_phase[3] = Manager3.ENGINE_manager();

    //    Serial.print(process_phase[0]);
    //    Serial.print(process_phase[1]);
    //    Serial.print(process_phase[2]);
    //    Serial.print(process_phase[3]);
    //    Serial.println("");

    state_all_done = process_phase[0] == 0 && process_phase[1] == 0 && process_phase[2] == 0 && process_phase[3] == 0;
    if (state_all_done == 1 || forced_stop == 1) // 모든 매니저의 작업이 끝나면 while 문 종료
    {
      Serial.println("-b");
      step_FR_displacement = Manager0.get_dispacement_from_pivot();
      step_FL_displacement = -Manager1.get_dispacement_from_pivot();
      step_RR_displacement = Manager2.get_dispacement_from_pivot();
      step_RL_displacement = -Manager3.get_dispacement_from_pivot();

      Manager0.set_measure_pivot();
      Manager1.set_measure_pivot();
      Manager2.set_measure_pivot();
      Manager3.set_measure_pivot();

      Serial.print("-f : ");
      Serial.print(step_FR_displacement);
      Serial.print(", ");
      Serial.print(step_FL_displacement);
      Serial.print(", ");
      Serial.print(step_RR_displacement);
      Serial.print(", ");
      Serial.println(step_RL_displacement);

      break;
    }
    //    Serial.println("driving");
    //    Serial.println(state_all_done);
    //    if (Serial.available() == true)
    //    {
    //      Serial.flush();
    //      break;
    //    }
    if ((micros() - time_before_feedback) > 100000) //delay를 쓰는 것은 다른 이벤트 처리를 방해할 우려가 큼.
    {
      time_before_feedback = micros();
      step_FR_displacement = Manager0.get_dispacement_from_pivot();
      step_FL_displacement = -Manager1.get_dispacement_from_pivot();
      step_RR_displacement = Manager2.get_dispacement_from_pivot();
      step_RL_displacement = -Manager3.get_dispacement_from_pivot();

      Manager0.set_measure_pivot();
      Manager1.set_measure_pivot();
      Manager2.set_measure_pivot();
      Manager3.set_measure_pivot();

      Serial.print("-f : ");
      Serial.print(step_FR_displacement);
      Serial.print(", ");
      Serial.print(step_FL_displacement);
      Serial.print(", ");
      Serial.print(step_RR_displacement);
      Serial.print(", ");
      Serial.println(step_RL_displacement);
    }
    while (1) //delay를 쓰는 것은 다른 이벤트 처리를 방해할 우려가 큼. 루프타임을 비교하는 부분
    {
      if ((micros() - time_before_control) > 4000)
      {
        //Serial.println("delay_test");
        time_before_control = micros();
        break;
      }
      //nothing
    }
  }
  Serial.println("-done");
}

//매인 루프 내에 무한 반복문이 있는 경우 SerialEvent가 작동하지 않을 수 있다고 함.
//https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=kiatwins&logNo=221121311465
//void serialEvent()
//{
//  Serial.println("123123");
//  serial_lock = 1;
//  string_input;
//  x = Serial.parseInt();
//  Serial.print(x);
//}
