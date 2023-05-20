#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
    //variables
  private:
    // 모터의 특성
    int stepsPerRev_org = 48;
    int gearRatio = 50;
    int stepsPerRev_gear = 48 * 50;

    //모터의 상태
    short motor_state = -1;// -1: park, 0: stop, 1: drive
    int rest_mode = 0;
    int step_state = 0;//0~7 : drivingMode

    int motor_out_bit = 0b0000;
    //    int gpioPort = -1;//아날로그 핀에대하여서도 digitalWrite()가 가능하므로 저수준 제어까지는 하지 않음.
    int pin_0 = -1;
    int pin_1 = -1;
    int pin_2 = -1;
    int pin_3 = -1;

    //구동 작업의 상태
    long int steps_input = 0;
    bool job_done = 0;
    long int step_stack = 0;
    long int step_stack_pivot = 0;//측정용 변수

    //함수들
  public:
    //상태 설정
    void set_gpio(int pin_a_, int pin_b_, int pin_a, int pin_b);

    void set_state(short state);
    
    void set_rest(short state);
    //작업 관리
    bool set_steps_input(long int steps);
        
    bool set_loadSteps();

    int get_is_drive_done();
    //정보 기록
    void set_pivot();
    
    void set_pivot(int pivot_val);

    long int set_step_on_stack(int step_stack_arg);
    
    //정보 반환
    long int get_dispacement_from_pivot();
    
    long int get_step_on_stack();
    //출력 보조
    void set_motor_pin_out_bit();

    void do_motor_pin_write_bit();
    //출력 핵심
    int ENGINE_drive();//완료 여부 값을 반환 , 0 엔진 작동중, 1 엔진 작동 완료, -1 작동오류

};

#endif


/*

  class Step_motor:
    def __init__(self):
        self.stepsPerRev_org = 48
        self.gearRatio = 50
        self.stepsPerRev_gear = 48*50

        self.__motor_state = -1# -1: park, 0: stop, 1: drive

        self.__steps_cumulated = 0#누적 각도 계산을 통한 위치 추적 방식은, 별도의 로컬라이제이션 기법이 사용될 수 있을 때 필요성이 감소한다.
        self.__steps_pivot = 0
        self.__steps_present = 0
        self.__steps_displacement = 0

        self.__step_state = 0
        self.__step_stack = 0

        self.__input_set_is_new = 0
        self.__set_encoder_now_reading = 0


        self.__motor_bit = 0b0000
        self.__a = -1
        self.__b = -1
        self.__c = -1
        self.__d = -1

  # ---------------------------------------------


    def init_cumulated(self):
        self.__steps_cumulated = 0

    def add_stepsCumulated(self, steps):
        self.__steps_cumulated += steps

    def get_steps_cumulated(self):
        return self.__steps_cumulated

    def set_steps_state(self, state):
        self.__step_state = state

    def get_steps_state(self):
        return self.__step_state


    #스템 읽기 함수는 모터가 1스텝이라도 움직인 순간 마다마다 불려지는 것이 가장 정확하다.
    #즉 모터 ENGINE 함수가 불려진 직후 바로 호출되는 것이 가장 최선
    def set_semapore_encoder_reading(self, bool_state):
        # 모터 스텝 누적을 통한 로커레아 제이션 부분의 구현
        self.__set_encoder_now_reading = bool_state

    def get_semapore_encoder_reading(self):
        return self.__set_encoder_now_reading

  # ---------------------------------------------
    def set_motor_pin_mode(self, pin1, pin2, pin3, pin4):-> set_gpio
        print("Set Motor Pins")
        self.__a = pin1
        self.__b = pin2
        self.__c = pin3
        self.__d = pin4
        print(pin1)
        GPIO.setup(self.__a, GPIO.OUT)
        GPIO.setup(self.__b, GPIO.OUT)
        GPIO.setup(self.__c, GPIO.OUT)
        GPIO.setup(self.__d, GPIO.OUT)

    def set_motor_pin_out(self):#full_step_mode
        #a', b', a, b
        if(self.__step_state == -1):
            self.__motor_bit = 0b0000
            self.__set_motor_pin_write_bit_mode()#exceptional method...
        #elif(self.__step_state == 0):
        #    self.__motor_bit = 0b0011
        #elif(self.__step_state == 1):
        #    self.__motor_bit = 0b0010
        #elif(self.__step_state == 2):
        #    self.__motor_bit = 0b0110
        #elif(self.__step_state == 3):
        #    self.__motor_bit = 0b0100
        #elif(self.__step_state == 4):
        #    self.__motor_bit = 0b1100
        #elif(self.__step_state == 5):
        #    self.__motor_bit = 0b1000
        #elif(self.__step_state == 6):
        #    self.__motor_bit = 0b1001
        #elif(self.__step_state == 7):
        #    self.__motor_bit = 0b0001
        elif(self.__step_state == 0):
            self.__motor_bit = 0b0011
        elif(self.__step_state == 1):
            self.__motor_bit = 0b0110
        elif(self.__step_state == 2):
            self.__motor_bit = 0b1100
        elif(self.__step_state == 3):
            self.__motor_bit = 0b1001
        elif(self.__step_state == 4):
            self.__motor_bit = 0b0011
        elif(self.__step_state == 5):
            self.__motor_bit = 0b0110
        elif(self.__step_state == 6):
            self.__motor_bit = 0b1100
        elif(self.__step_state == 7):
            self.__motor_bit = 0b1001
        self.__set_motor_pin_write_bit_mode()
        #print(self.__motor_bit)

    def __set_motor_pin_write_bit_mode(self):
        motor_bit = self.__motor_bit
        a_bit = (motor_bit>>0)%0b10
        b_bit = (motor_bit>>1)%0b10
        c_bit = (motor_bit>>2)%0b10
        d_bit = (motor_bit>>3)%0b10
        GPIO.output(self.__a, a_bit)
        GPIO.output(self.__b, b_bit)
        GPIO.output(self.__c, c_bit)
        GPIO.output(self.__d, d_bit)

    def set_steps_displacement(self, steps):
        self.__steps_displacement = steps
        self.__input_set_is_new = 1
        # print("aaa", self.__steps_displacement)

    def set_loadSteps(self):
        self.__step_stack += self.__steps_displacement
        # print(self.__step_stack)

    def get_step_stack(self):
        return self.__step_stack

    def ENGINE_drive(self):
        if(self.__step_stack != 0):
            self.__motor_state = 1
            if(self.__step_stack > 0):
               #모터 반시계 방향 회전
                self.__step_state += +1
                if(self.__step_state >= 8):
                    self.__step_state = 0
                self.set_motor_pin_out()

                self.__step_stack += -1
                self.add_stepsCumulated(+1)

            elif(self.__step_stack < 0):
                #모터 시계 방향 회전
                self.__step_state += -1
                if(self.__step_state < 0):
                    self.__step_state = 7
                self.set_motor_pin_out()

                self.__step_stack += +1
                self.add_stepsCumulated(-1)
        else:
            self.__steps_cumulated += 0
            self.__motor_state = 0

    def set_motorState(self, state):
        self.__motor_state = state

    def get_motorState(self):
        return self.__motor_state




   /
*/
