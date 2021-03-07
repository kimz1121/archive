/*
 * robot_arm_control_4.cpp
 *
 * Created: 2020-02-16 오후 11:21:51
 * Author : kimzt
 */ 

/*
signal code list

0 : natural
1 : forward
2 : backward
3 : stop

101, 'e' : error

0xff, ob11111111 : waiting,(will be fine soon)

*/


#define F_CPU 16000000UL //사용중인 크리스털 값을 상수로 설정줍니다. (16MHz)
#include <avr/io.h>
#include <avr/interrupt.h> //인터럽트 관련 헤더파일을 포함합니다.
#include <util/delay.h>
//<시작>========================================

struct motor_mathod
	{
		//바뀌기 어려운 기계고유적 특성 
		int motor_type;//'s' step,'b' bldc
		int presison;
		
		//기계고유적 특성과 사용자의 임의 지정의 사이
		char proper_direction;
		char  initialization_value;
		
		//사용자가 임의로 설치한 특성
		char motor_port;
		int motor_pin;
		int hall_sensor_pin;
		
		//기계부품 제어 특성
		char motor_driving_mode;//stop or natural.
		
		//하드웨어적 특성을 실시간으로 저장하기 위한 변수
		//general, 공통의
		char error;
		int cumulated_steps;
		
		
		//hall_sensor_only, 홀센서 전용
		int current_hall_sensor_value;// = -1
		int just_before_hall_sensor_value;// = -1
		
		char motor_own_bit_new;// = -1
		char motor_own_bit_old;// = -1
		
		//프로그램적 요인으로 모터 별로 빈번하게 활용되는 변수로 구조체에 구속 시켰음.
		
		int microscopic_displacement_value;
		int direction_value;
		
		double progress_situation;// = 0//진행상황
		char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		
		long macroscopic_destination_position;// = 0//거시적 경로 설정 함수에서 활용
		long macroscopic_current_position;// = 0
		
		long microscopic_destination_position;// = 0//미시적 경로 설정 함수에서 사용
		long microscopic_current_position;// = 0
		
	};
struct motor_list
{
	
	struct motor_mathod m1_stepper;//제 1관절 모터
	struct motor_mathod m2_bldc;//제 2관절 모터
	struct motor_mathod m3_stepper;//제 3관절 모터
	struct motor_mathod m4_stepper;//제 4관절 모터
	void* ending_NULL_pointer;// = NULL
	
	double stream_of_time;
	
	long current_position_x;
	long current_position_y;
	long current_position_z;
	
	long destination_position_x;
	long destination_position_y;
	long destination_position_z;
	
	int munber_of_motors;
	
}robot_arm_1;

//ending_NULL_pointer = NULL;

//<시작>헤더 정리================================
int encoder_management(struct motor_mathod* motor_name);
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress);
int port_input_assistant(char port_name, int object_pin);
char bit_analysis(int bit_old, int bit_new);//default parameter : bit_old = 0b000, bit_new = 0b000
//<끝>헤더 정리==================================



//<끝>==========================================
//<시작>========================================
//timer ISR part <begin> 
ISR(TIMER0_COMP_vect)//타이머 인터럽트 부분, 홀센서 값 읽기 스텝수 누계 계산 등
{
	//hall_sensor reading and position management
	//<시작>========================================
	//이부분은 인터럼트가 실행된이후 동작하는 부분.//일단 타이머 인터럽트를 이용합시다.그리고 '속도 제한'을 둡시다. 매 타임아웃마다 모든 엔코더의 스텝 변화값을 기록하고 누계에 넣읍시다.
	encoder_management(&robot_arm_1.m2_bldc);
	//int* motor_list, int* motor_name,
	//return consumed time






	//<끝>==========================================
}
//<끝>========================================
//function for timer_interrupt only
int encoder_management(struct motor_mathod* motor_name)
{
	sicuential_bit_input(port_input_assistant((*motor_name).motor_port, (*motor_name).hall_sensor_pin), &((*motor_name).current_hall_sensor_value), &((*motor_name).just_before_hall_sensor_value));
	switch(bit_analysis((*motor_name).just_before_hall_sensor_value, (*motor_name).current_hall_sensor_value))
	{
		case 3:
			(*motor_name).cumulated_steps = (*motor_name).cumulated_steps + 1;
			break;
		case 2:
			(*motor_name).cumulated_steps = (*motor_name).cumulated_steps - 1;
			break;
		case 'e':
			(*motor_name).error = 'e';
			break;
	}
}
//<통신부 시작>===============================================
void UART1_init(void)
{
	UCSR1A |= (1 << U2X1);
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
	UBRR1H = 0;
	UBRR1L = 207;
}
void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}
unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1 << RXC1)));
	return UDR1;
}
//<통신부 끝>===============================================
















//기본 분석 함수
//<시작>========================================
char find_coupled_nuber(int bit)
{
	//-1 or 2 is error.
	char discriminant = (bit>>2) + ((bit%0b100)>>1) + (bit%0b10) - 1;
	if(discriminant == 1 || discriminant == 0)
	{
		return discriminant;
	}
	else
	{
		//printf("an error occurred, the bit sign wasn't good.\n");
		return 'e';
	}
}
//<끝>==========================================
//<시작>========================================
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress)//깊은 복사를 위해서 주소를 받음//it takes adress for deep copy
{
	//바뀌지 않은경우에도 함수가 실행되어 old와 new가 서로 같은 값이 되어 버리는 상황을 방지하는 기능 추가
	if(*bit_new_s_adress == -1)//초기 값 검사를 -1로 하는 것이좋을까?
	{
		*bit_new_s_adress = bit_input;
		return 0;
	}
	/*
	if(*bit_new_s_adress == bit_input)
	{
		return 0;
	}
	*/
	*bit_old_s_adress = *bit_new_s_adress;
	*bit_new_s_adress = bit_input;
	return 1;
}
//<끝>==========================================
//<시작>========================================
int object_binary_making(int object_pin, int binary_int)
{
	int result_binary = 0;
	int not_binary = ~object_pin;
	result_binary |= (object_pin & binary_int);
	result_binary &= (not_binary | binary_int);
	//printf("%d\n", result_binary);
	return result_binary;
}
//<끝>==========================================
/*
int binary_fit_modifing(int object_pin, int binary_int, int object_bin_length, int signal_binary_length)
{
	
}
*/













//기본 동작 함수.
//<시작>========================================
void delay_costom_ms(int time_ms = 0)
{
	int n = 0;
	while(n < time_ms)
	{
		_delay_ms(1);
		n=n+1;
	}
}
//<끝>==========================================

//말단 입출력 보조 함수
//<시작>========================================
int port_input_assistant(char port_name, int object_pin)
{
	int temp = 0;
	int result = 0;
	switch(port_name)
	{
		case 'a':
		case 'A':
		temp = PINA;
		break;
		case 'b':
		case 'B':
		temp = PINB;
		break;
		case 'c':
		case 'C':
		temp = PINC;
		break;
		case 'd':
		case 'D':
		temp = PIND;
		break;
		case 'e':
		case 'E':
		temp = PINE;
		break;
		case 'f':
		case 'F':
		temp = PINF;
		break;
	}
	int i = 0;
	int n = 0;
	int array[8] = {0,};
	for(i = 0; i < 8; i = i + 1)
	{
		if(((object_pin>>i)%0b10))
		{
			array[n] = i;
			n = n + 1;
		}
	}
	for(i = 0; i < n; i = i + 1)
	{
		result |= ((temp>>array[i])%0b10)<<i;
	}
	return result;
}
//<끝>==========================================
//<시작>========================================
void port_output_assistant(char port_name, int object_pin, int binary_int = 0b00000000)
{
	switch(port_name)
	{
		case 'a':
		case 'A':
		PORTA = object_binary_making(object_pin, binary_int);
		break;
		case 'b':
		case 'B':
		PORTB = object_binary_making(object_pin, binary_int);
		break;
		case 'c':
		case 'C':
		PORTC = object_binary_making(object_pin, binary_int);
		break;
		case 'd':
		case 'D':
		PORTD = object_binary_making(object_pin, binary_int);
		break;
		case 'e':
		case 'E':
		PORTE = object_binary_making(object_pin, binary_int);
		break;
		case 'f':
		case 'F':
		PORTF = object_binary_making(object_pin, binary_int);
		break;
	}
	//printf("%c : %d\n", port_name, object_binary_making(object_pin, binary_int));
}
//<끝>==========================================





















//기계 분석 관련 함수
//<시작>========================================
char bit_analysis(int bit_old = 0b000, int bit_new = 0b000)//it return 0, 2, 3, or e. 0 means it isn't moved, 2 means forward, 3 means backward, e means error.
{
	//(bit>>2) + ((bit%0b100)>>1) + (bit%0b10), (bit>>2), ((bit%0b100)>>1), (bit%0b10) //자리수 합산 코드...
	char bit_u_new = (bit_old>>2);
	char bit_v_new = ((bit_old%0b100)>>1);
	char bit_w_new = (bit_old%0b10);
	
	char bit_u_old = (bit_new>>2);
	char bit_v_old = ((bit_new%0b100)>>1);
	char bit_w_old = (bit_new%0b10);

	
	if(find_coupled_nuber(bit_old) == 'e' || find_coupled_nuber(bit_new) == 'e' )
	{
		//printf("an error occurred, the return sign from a function \'find_coupled_nuber(int bit)\' wasn't good.\n");
		return 'e';
	}
	
	char discriminant =
	((find_coupled_nuber(bit_old)^bit_u_new)&&(1^(bit_u_new^bit_u_old)))*((find_coupled_nuber(bit_old)^bit_v_old)*(2)+(find_coupled_nuber(bit_old)^bit_w_old)*(3))
	+((find_coupled_nuber(bit_old)^bit_v_new)&&(1^(bit_v_new^bit_v_old)))*((find_coupled_nuber(bit_old)^bit_w_old)*(2)+(find_coupled_nuber(bit_old)^bit_u_old)*(3))
	+((find_coupled_nuber(bit_old)^bit_w_new)&&(1^(bit_w_new^bit_w_old)))*((find_coupled_nuber(bit_old)^bit_u_old)*(2)+(find_coupled_nuber(bit_old)^bit_v_old)*(3))
	//+6*((bit_u_new^bit_u_old)||(bit_v_new^bit_v_old)||(bit_w_new^bit_w_old))*(find_coupled_nuber(bit_old)^bit_u_new)*(find_coupled_nuber(bit_old)^bit_v_new)*(find_coupled_nuber(bit_old)^bit_w_new)
	+(((find_coupled_nuber(bit_old)^bit_u_new)&&(bit_u_new^bit_u_old))+((find_coupled_nuber(bit_old)^bit_v_new)&&(bit_v_new^bit_v_old))+((find_coupled_nuber(bit_old)^bit_w_new)&&(bit_w_new^bit_w_old)))*6
	;
	
	if(discriminant == 6)
	{
		//printf("an error occurred, argument that had been inputed, wasn't good\nther are some potential of step_out\n");
		return 'e';
	}
	else
	{
		return discriminant;
	}
}
//<끝>==========================================
//<시작>========================================
//this function calculate hall sensor signals and than return output signals by situation of diretion_value.
int sign_calculating(int hall_sensor_value, char diretion_value)//int object_pin 계산하는 기능 추가하기.
{
	int value = -1;
	int value_shifted = -1;
	char tranceformed_diretion_value = 'e';
	//direction_value can only have 3 status that is 'forward', 'backward', 'stop', 'natural'


	switch(diretion_value)
	{
		case 0://natural//its return value is being considered 'N'=~'n' or 0b000000 = 0;
		return 0;
		break;
		case 1://forward
		tranceformed_diretion_value = 1;
		break;
		case 2://backward
		tranceformed_diretion_value = -1;
		break;
		case 3://stop
		tranceformed_diretion_value = 0;
		break;
	}
	
	switch(hall_sensor_value)
	{
		case 0b110:
		value = 1;
		break;
		case 0b100:
		value = 2;
		break;
		case 0b101:
		value = 3;
		break;
		case 0b001:
		value = 4;
		break;
		case 0b011:
		value = 5;
		break;
		case 0b010:
		value = 6;
		break;
		default:
		return 'e';
	}

	//printf("%d\n", value);
	value_shifted = value + tranceformed_diretion_value;//shift bit at here
	//printf("%d\n", value_shifted);
	switch(value_shifted%6)
	{
		case 1:
		return 0b100100;
		break;
		
		case 2:
		return 0b100001;
		break;
		
		case 3:
		return 0b001001;
		break;
		
		case 4:
		return 0b011000;
		break;
		
		case 5:
		return 0b010010;
		break;
		
		case 0://6th value
		return 0b000110;
		break;
		
		default:
		return 'e';
	}
}
//<끝>==========================================
//<시작>========================================
int virtual_hall_value_generator(int* hallsensor_value_adress, long position_value, int diretion)//tester_function
{
	int value = 0;
	 switch(position_value%6)
	 {
		 case 1:
			value = 0b110;
			break;
		 
		 case 2:
			 value = 0b100;
			 break;
		 
		 case 3:
			 value = 0b101;
			 break;
		 
		 case 4:
			 value = 0b001;
			 break;
		 
		 case 5:
			 value = 0b011;
			 break;
		 
		 case 0://6th value
			 value = 0b010;
			 break;
	 }
	 *hallsensor_value_adress = value;
	 return value;
}
//<끝>==========================================
//<시작>========================================
void virtual_position_management(long* current_position, int direction_value)//use it with delay. 
{
	switch(direction_value)
	{
		case 1:
			*current_position = *current_position + 1;
			break;
		case 2:
			*current_position = *current_position - 1;
			break;
	}
}
//<끝>==========================================
//<시작>========================================
void current_position_caliberation(struct motor_mathod* motor_name)
{
	(*motor_name).microscopic_current_position = 0;
	(*motor_name).initialization_value = 1;
}
//<끝>==========================================
/*
//<시작>========================================
int motor_data_calculater(struct motor_mathod* motor_name, int pointer_shift_variance)
{
	(*(motor_name) + pointer_shift_variance)
}
//<끝>==========================================
*/
//<시작>========================================
int direction_value_clac_arg_3(long begin_point, long last_point, int driving_mode)
{
	int direction_value = -1;
	if(last_point - begin_point == 0)
	{
		switch (driving_mode)
		{
			case 'n':
			case 'N':
				direction_value = 0;
				break;
		
			case 's':
			case 'S':
				direction_value = 3;
				break;
		}
	}
	else
	{
		if(last_point - begin_point > 0)
		{
			direction_value = 1;
		}
		else//the situation of last_point - begin_point < 0
		{
			direction_value = 2;
		}
	}
	return direction_value;
}
//<끝>==========================================
//<시작>========================================
int direction_value_clac_arg_2(int displacement, int driving_mode)
{
	int direction_value = -1;
	if(displacement == 0)
	{
		switch (driving_mode)
		{
			case 'n':
			case 'N':
			direction_value = 0;
			break;
			
			case 's':
			case 'S':
			direction_value = 3;
			break;
		}
	}
	else
	{
		if(displacement > 0)
		{
			direction_value = 1;
		}
		else//the situation of last_point - begin_point < 0
		{
			direction_value = 2;
		}
	}
	return direction_value;
}
//<끝>==========================================
//<시작>========================================
long displacement_calc(long begin_point, long last_point)
{
	return last_point - begin_point;// last - begin is positive 
}
//<끝>==========================================





















//기계 입출력 관련함수
//<시작>========================================
void output_safety_function(char port_name ,int object_pin)//모터용 출력으로 사용되는 핀레지스터만 제어하는 기능 추가 필요.!!!!!!!!!!!!!!!!!!!!!!!!!
{
	port_output_assistant(port_name, object_pin, 0b0000000);
	_delay_ms(5);
}
//<끝>==========================================
//<시작>========================================
int hall_sensor_reding(char port_name, int pin_data = 0b00000111)
{
	return  port_input_assistant(port_name, pin_data);
}
//<끝>==========================================
//<시작>========================================
void input_value_to_brige_open_loop(char port_name, int object_pin,int binary_int = 0b00000000, int time_ms = 0)//a part of auto output system.
{
	port_output_assistant(port_name, object_pin, binary_int);
	delay_costom_ms(time_ms);//it should be changed, it can control several of motors in same time. doesn't motors are stopped by that delay function.
	output_safety_function(port_name, object_pin);
}
//<끝>==========================================
//<시작>========================================
void step_motor_driving_open_loop(int motor_port, int motor_pin, int* direction_value)
{
	port_output_assistant(motor_port, motor_pin, *direction_value);
	//current_position_value 증감시키기.
}
//<끝>==========================================
//<시작>========================================
void bldc_motor_driving_open_loop(char port_name, int object_pin, int* hall_sensor_value, int diretion_value)
{
	int output_signal = 0;
	output_signal = sign_calculating(*hall_sensor_value, diretion_value);
	//printf("체크 7 : %d\n", output_signal);
	input_value_to_brige_open_loop(port_name, object_pin, output_signal, 50);//anqngk tkdxodptj 20 ms 동안 5v내외의 전압으로 탈조 없이 진행 되었음.... 그러나 부하가 있는 상태에서는 상태에 따라 필요한 전력 공급 시간이 다를 수 있으니 모터의 상태에 따른 피드백 제어가 필요함... 증 스텝이 1(or -1)증가 할때까지 신호를 공급해야 하는 것임.
	
	/*
	//오픈 루프 제어시 완료이후 현재 스텝 누계를 증감시키는 함수 가 필요함
	int open_loop_cumulating(int* current_step, int direction = 0, int variance = 0)
	{
		switch(diretion_value)
		{
			case -1:
				break;
			case 1:
				variance == 1;
				berak;
			case 2:
				variance == -1;
				break;
			default://situation of direction == 0 or direction == 3
				return 0;
		}
	}
		*current_step = *current_step + variance;
	}
	*/
}
//<끝>========================================
//<시작>========================================
void bldc_motor_driving_closed_loop(char port_name, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* diretion_value)
{
	while(bit_analysis(*just_before_hall_sensor_value, *current_hall_sensor_value) == 0)
	{
		port_output_assistant(port_name, motor_pin, sign_calculating(*current_hall_sensor_value, *diretion_value));
	}
	output_safety_function(port_name, motor_pin);
}
//<끝>========================================
//<시작>========================================
void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* direction_value)//, char hall_sensor_port, int hall_sensor_pin 등 홀센서 값읽기는 실시간 감시가 중요하기에 타이머 인터럽트가 대신 담당한다. 
{
	//이 함수에서 스텝모터/bldc, 강제 홀딩 여부, 포트및 핀설정, 방향 , 모든 것을 판단하여 말단 함수들에게 명령을내림.
	switch(motor_type)
	{
		case 'b':
		case 'B':
			bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value, just_before_hall_sensor_value, direction_value);//파라메터를 받은 순간의 값과 함수가 실행중일 때의 값이 서로 다를 수 있기때문에 타이머 인터럽트로 계속 초기화 하는 값을 메모리 주소와 포인터를 이용하여 전달한다.
			break;
		case 's':
		case 'S':
			step_motor_driving_open_loop(motor_port, motor_pin, direction_value);//딜레이 타임도 넣어러주면 선세한 모터 제어에 좋을듯. 단 구조체에 미리 저장해두어 모터의 속성을 가져다 쓰듯이 사용하기. 
			break;
	}
}
//<끝>========================================
//<시작>========================================
void motor_combo_serise_drivig(struct motor_list* robot_arm_adress, int number_of_motor)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_adress;
	
	int n = 0;
	
	while(n < 4)
	{	
		if((*motor_Ptr).microscopic_complition == -1)
		{
			general_motor_drivig((*motor_Ptr).motor_type, (*motor_Ptr).motor_port, (*motor_Ptr).motor_pin, &((*motor_Ptr).current_hall_sensor_value), &((*motor_Ptr).just_before_hall_sensor_value), &((*motor_Ptr).direction_value));
		}
		motor_Ptr++;
		n = n + 1;
	}
}
//<끝>========================================




















//경로설정 관련 함수.


//경로계산 관련 함수.
//<시작>========================================
void rectangular_to_digree(long position_x, long position_y, long position_z, long)
{
	
}
//<끝>========================================
//<시작>========================================
//어깨부분과 팔 부분을 구분하여 계산을 나누기.
//팔이 가져야할 기구적 형태는 3차원 적 길이에 따라 나타남. 절대적인 위치에 대해서는 상대적으로 자유로운 편
//길이와 관절 개수 만으로 기계관절의 형태를 정할 수 있나??
//<끝>========================================
//<시작>========================================
int current_position_management()
{
	
}
//<끝>========================================
//<시작>========================================
int current_time_managemant()
{
	
}
//<끝>========================================
//<시작>========================================
int rectangular_coordinates_first_class_order()
{
	
}
//<끝>========================================
//<시작>========================================
int rectangular_coordinates_macroscopic_to_micro_ordering(int motor_pin, int* hall_sensor_value, int steps, int speed)
{
	
}
//<끝>========================================
//<시작>========================================
int rectangular_coordinates_microscopic_to_driver_ordering(int motor_pin, int* hall_sensor_value, int steps, int speed)
{//모터 드리이버는 직교 좌표계의 형태로 움직이더라고 sin, cos tan 등 삼각 함수의 형태를 취하고 있음으로 무의미
	
}
//<끝>========================================
//<시작>========================================
void first_class_order(struct motor_mathod* motor_name, long position)
{
	if((*motor_name).initialization_value == 1)
	{
		(*motor_name).macroscopic_destination_position = position;
		(*motor_name).macroscopic_current_position = (*motor_name).microscopic_current_position;
	}
}
//<끝>========================================
//<시작>========================================
int number_line_macroscopic_to_micro_ordering(struct motor_mathod* motor_name, long destination, double current_time, double time_out)
{
	//printf("asass\n");
	(*motor_name).microscopic_destination_position = (*motor_name).macroscopic_current_position + ((*motor_name).macroscopic_destination_position - (*motor_name).macroscopic_current_position)*(current_time/time_out);
}
//<끝>========================================
//<시작>========================================
int number_line_microscopic_to_driver_ordering(struct motor_mathod* motor_name)
{
	(*motor_name).microscopic_displacement_value = (*motor_name).macroscopic_destination_position - (*motor_name).macroscopic_current_position;
	(*motor_name).direction_value = direction_value_clac_arg_2((*motor_name).microscopic_displacement_value, (*motor_name).motor_driving_mode);
}	
//<끝>========================================


















//동작완료 점검 함수
int macroscopic_compliment_checking(void* robot_arm_adress)//새로운 경로를 받기전에 경로가 완료 되었는지 확인하기 위해 쓰임.//이곳에서 경로와의 거리차가 +인지 -인지도 계산하기
{
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_adress;
	while(n < 4)
	{
		if((*motor_Ptr).macroscopic_destination_position == (*motor_Ptr).microscopic_current_position)
		{
			(*motor_Ptr).macroscopic_current_position = (*motor_Ptr).microscopic_current_position;
			
		}
		if((*motor_Ptr).macroscopic_current_position != (*motor_Ptr).macroscopic_destination_position)
		{
			(*motor_Ptr).macroscopic_complition = -1;
			m = m + 1;
		}
		else
		{
			(*motor_Ptr).macroscopic_complition = 1;
		}
		motor_Ptr++;
		n = n + 1;
	}
	if(m != 0)
	{
		return -1;
	}
	else
	{
		robot_arm_1.stream_of_time = 0;
		return 1;
	}
}
//<끝>========================================
//<시작>========================================
int microscopic_compliment_checking(void* robot_arm_adress)//새로운 경로를 받기전에 경로가 완료 되었는지 확인하기 위해 쓰임.
{
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_adress;
	while(n < 4)
	{
		if(displacement_calc((*motor_Ptr).microscopic_current_position, (*motor_Ptr).microscopic_destination_position) != 0)
		{
			(*motor_Ptr).microscopic_complition = -1;
			(*motor_Ptr).microscopic_displacement_value = displacement_calc((*motor_Ptr).microscopic_current_position, (*motor_Ptr).microscopic_destination_position);
			m = m + 1;
		}
		else
		{
			(*motor_Ptr).microscopic_complition = 1;
		}
		motor_Ptr++;
		n = n + 1;
	}
	if(m != 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
	
}
//<끝>========================================
//<시작>========================================



/*
int main(void)
{
	current_position_caliberation(&robot_arm_1.m2_bldc);
	int a = 0;
	//		printf("위치 초기화\n");
	while(1)
	{
		a = a + 1;
		if(a%2 == 0)
		{
			first_class_order(&robot_arm_1.m2_bldc, 200);
		}
		else
		{
			first_class_order(&robot_arm_1.m2_bldc, 0);
		}
		//		printf("현재위치 : %d\n", robot_arm_1.m2_bldc.macroscopic_current_position);
		//		printf("체크1 : %d\n", robot_arm_1.m2_bldc.macroscopic_destination_position);
		//		printf("체크2 : %d\n", macroscopic_compliment_checking(&robot_arm_1));
		
		while(macroscopic_compliment_checking(&robot_arm_1) == -1)
		{
			//			printf("현재위치 : %d\n", robot_arm_1.m2_bldc.macroscopic_current_position);
			number_line_macroscopic_to_micro_ordering(&robot_arm_1.m2_bldc, 30, robot_arm_1.stream_of_time, 30);
			//			printf("체크3 : %d\n", robot_arm_1.m2_bldc.microscopic_destination_position);
			while(microscopic_compliment_checking(&robot_arm_1) == -1)
			{
				
				number_line_microscopic_to_driver_ordering(&robot_arm_1.m2_bldc);//direction value 계산완료 시점.
				//microscopic_compliment_checking()
				virtual_hall_value_generator(&robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
				bldc_motor_driving_open_loop('D', 0b00111111, &robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.direction_value);
				virtual_position_management(&robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
				printf("체크5 : %d\n", robot_arm_1.m2_bldc.microscopic_current_position);
				printf("체크5 : %d\n", robot_arm_1.m2_bldc.microscopic_destination_position);
				printf("체크6 : %d\n", robot_arm_1.m2_bldc.direction_value);
			}
			robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
			_delay_ms(500);
		}
		
		//		printf("체크4 : %d\n", robot_arm_1.stream_of_time);
		//		if(robot_arm_1.stream_of_time > 60)
		//		{
		//			robot_arm_1.stream_of_time = 0;
		//			printf("시간 초기화\n");
		//		}
	}
}
*/



int main(void)
{
	
	UART1_init();
	//<시작>변수 초기값 설정=========================================
	robot_arm_1.m2_bldc.motor_driving_mode = 'n';
	robot_arm_1.m2_bldc.presison = 1;
	robot_arm_1.m2_bldc.proper_direction = 1;
	
	robot_arm_1.m2_bldc.motor_port = 'b';
	robot_arm_1.m2_bldc.motor_pin = 0b00111111;
	robot_arm_1.m2_bldc.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m2_bldc.current_hall_sensor_value = -1;
	robot_arm_1.m2_bldc.just_before_hall_sensor_value = -1;
	//<끝>변수 초기값 설정===========================================
	//<시작>타이머 인터럽트 조건 설정=================================
	TCCR0 = (1<<WGM00) | (1<<CS0);
	
	TCNT0 = 0;
	
	OCR0 = 10;
	
	TIMSK |= (1<<OCIE0);
	
	sei();
	//<끝>타이머 인터럽트 조건 설정===================================
	
	
	current_position_caliberation(&robot_arm_1.m2_bldc);
		first_class_order(&robot_arm_1.m2_bldc, 30);
	while(1)
	{
		UART1_transmit(robot_arm_1.m2_bldc.cumulated_steps);
	}
}
//<끝>========================================
