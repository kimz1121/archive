void _delay_ms(int value)
{
 	 
}
void _delay_us(int value)
{
	
}

#define PINA 0;
#define PINB 0;
#define PINC 0;
#define PIND 0;
#define PINE 0;
#define PINF 0;

char DDRA = 0;
char DDRB = 0;
char DDRC = 0;
char DDRD = 0;
char DDRE = 0;
char DDRF = 0;

char PORTA = 0;
char PORTB = 0;
char PORTC = 0;
char PORTD = 0;
char PORTE = 0;
char PORTF = 0;

#include <stdio.h>

/*
 * step_motor_test_4.cpp
 *
 * Created: 2020-02-29 오후 9:58:35
 * Author : khk
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

//<전처리>00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
#define F_CPU 16000000UL //사용중인 크리스털 값을 상수로 설정줍니다. (16MHz)
//#include <avr/io.h>
//#include <avr/interrupt.h> //인터럽트 관련 헤더파일을 포함합니다.
//#include <util/delay.h>
#include <math.h>
//<시작>========================================


//<구조체 정의>111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111


struct position_management
{
	//ractangular_coordinates
	double macroscopic_current_position_x = 0;
	double macroscopic_current_position_y = 0;
	double macroscopic_current_position_z = 0;

	double macroscopic_destination_position_x = 0;
	double macroscopic_destination_position_y = 0;
	double macroscopic_destination_position_z = 0;

	//위치를 각도로 부터 역계산 해야 하는 부분으로 현재는 구현 하지 않음. 2020.02.22
	double microscopic_current_position_x = 0;
	double microscopic_current_position_y = 0;
	double microscopic_current_position_z = 0;
	
	double transformed_current_position_x = 0;
	double transformed_current_position_y = 0;
	double transformed_current_position_z = 0;
	
	//========================================================================

	double microscopic_destination_position_x = 0;
	double microscopic_destination_position_y = 0;
	double microscopic_destination_position_z = 0;
	
	double transformed_destination_position_x = 0;
	double transformed_destination_position_y = 0;
	double transformed_destination_position_z = 0;
};

struct function_part
{
	//double seta = 0.5*M_PI;

	double length = 10;
	
	//function_part_control
	//seta_value.
	double macroscopic_current_position_seta = 0;
	double macroscopic_destination_position_seta = 0;
	
	double microscopic_current_position_seta = 0;
	double microscopic_destination_position_seta = 0;
	
	//grip_value.
	double macroscopic_current_position_grip = 0;
	double macroscopic_destination_position_grip = 0;
	
	double microscopic_current_position_grip = 0;
	double microscopic_destination_position_grip = 0;

}func_gripper;

struct motor_mathod
{
	//바뀌기 어려운 기계고유적 특성
	int motor_type = 'v';//'s' step,'b' bldc
	int step_per_revolution_presison;//임시값 100.
	
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
	
	//모터 제어 및 구동 관련.
	int microscopic_displacement_value;
	char direction_value;

	double progress_situation;// = 0//진행상황
	char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		
	//<시작>직교제어가 아닌 각도 제어에 필요한 변수.===========
	long macroscopic_destination_position;// = 0//거시적 경로 설정 함수에서 활용
	long macroscopic_current_position;// = 0
	
	long microscopic_destination_position;// = 0//미시적 경로 설정 함수에서 사용
	long microscopic_current_position;// = 0
	//<끝>==================================================
	
	//직교제어에서 이용되는 변수.
	double current_degree = 0;
	double destination_degree = 0;
	long displacement_step;
	long current_step;
	long destination_step;
};

struct motor_list
{
	struct motor_mathod m1_stepper;//제 1관절 모터
	struct motor_mathod m2_stepper;//제 2관절 모터_step_motor_mode
	//struct motor_mathod m2_bldc;//제 2관절 모터
	struct motor_mathod m3_stepper;//제 3관절 모터
	struct motor_mathod m4_stepper;//제 4관절 모터
	struct motor_mathod m5_servo_gripper;
	void* ending_NULL_pointer;// = NULL
	
	double length_1 = 27;
	double length_2 = 30;
	double length_3 = 30;
	double length_4_func_part = 10;
	

	double stream_of_time;
	double time_out;
	
	char ractangular_macro_compliment_value;//'y' is done, 'n' is on progress.
	char ractangular_micro_compliment_value;
	
	struct position_management position_value;

	double cumulated_angle_position;
	
	int munber_of_motors;

}robot_arm_1;
//ending_NULL_pointer = NULL;

//<헤더 정리>222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222

//<시작>헤더 정리================================
int encoder_management(struct motor_mathod* motor_name);
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress);
//char port_input_assistant(char port_name, char object_pin);
char bit_analysis(int bit_old, int bit_new);//default parameter : bit_old = 0b000, bit_new = 0b000
//<끝>헤더 정리==================================

//<통신부>333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333

//<통신부 시작>===============================================
/*
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
*/
//<통신부 끝>===============================================

//<기본 분석 함수>44444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444
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
int object_binary_making(char port_value,char object_pin, char modified_binary)
{
	
	char result_binary = port_value;
	char intermediate_binary = 0;
	
	char not_binary = ~object_pin;
	intermediate_binary |= (object_pin & modified_binary);
	intermediate_binary &= (not_binary | modified_binary);
	//printf("%d\n", result_binary);
	
	result_binary &= not_binary;
	result_binary |= intermediate_binary;
	return result_binary;
}
//<끝>==========================================

char char_binary_fit_modifing(char object_pin, char binary_char)//, int object_bin_length = 8)//, int signal_binary_length = 3)
{
	// gkatnsms
	char modified_binary = binary_char;
	char temp = object_pin;
	while((temp % 0b10) == 0)
	{
		modified_binary = modified_binary << 1;
		temp = temp >> 1;
	}
	return modified_binary;
}



//<기본 동작 함수>55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555
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
int port_input_assistant(char port_name, char object_pin)
{
	int temp = 0;
	char result = 0;
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
void port_output_assistant(char port_name, char object_pin, char binary_char)
{
	
	switch(port_name)
	{
		case 'a':
		case 'A':
		PORTA = object_binary_making(PORTA, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
		case 'b':
		case 'B':
		PORTB = object_binary_making(PORTB, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
		case 'c':
		case 'C':
		PORTC = object_binary_making(PORTC, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
		case 'd':
		case 'D':
		PORTD = object_binary_making(PORTD, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
		case 'e':
		case 'E':
		PORTE = object_binary_making(PORTE, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
		case 'f':
		case 'F':
		PORTF = object_binary_making(PORTF, object_pin, char_binary_fit_modifing(object_pin, binary_char));
		break;
	}
	//printf("z");
	//printf("%c : %d\n", port_name, object_binary_making(object_pin, char_binary_fit_modifing(object_pin, binary_char)));
}
//<끝>==========================================



//<기계 분석 관련 함수>66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666
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
/*
void current_position_caliberation(struct motor_mathod* motor_name)
{
	(*motor_name).microscopic_current_position = 0;
	(*motor_name).initialization_value = 1;
*/
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
int direction_value_clac_arg_2(int displacement, int driving_mode)
{
	int direction_value = 'e';
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
/*
long displacement_calc(long begin_point, long last_point)// 앞으로 사용안함.
{
	return last_point - begin_point;// last - begin is positive 
}
*/
//<끝>==========================================

//<가상 및 테스트용 함수 정리>77777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
//가상 위치 계산 함수
//<시작>========================================
void virtual_position_management(long* current_position, int direction_value)//use it with delay.
{
	switch(direction_value)
	{
		case 1:
		//printf("forward\n");
		//printf("current position : %d\n", *current_position);
		*current_position = *current_position + 1;
		break;
		case 2:
		//printf("backward\n");
		//printf("current position : %d\n", *current_position);
		*current_position = *current_position - 1;
		break;
	}
}
//<끝>========================================
//가상 신호 계산 함수
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

//<경로계산 관련 함수>88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
//<시작>========================================
//어깨부분과 팔 부분을 구분하여 계산을 나누기.
//팔이 가져야할 기구적 형태는 3차원 적 길이에 따라 나타남. 절대적인 위치에 대해서는 상대적으로 자유로운 편
//길이와 관절 개수 만으로 기계관절의 형태를 정할 수 있나??
//<끝>========================================
//<시작>========================================
int current_time_managemant()//현재 쓰이지 않음.
{
	
}
//<끝>========================================
//<시작>========================================
//필요한 함수 원형 선언
int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address);

void combine_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, double purpose_time_out)
{
	if(rectangular_coordinate_macroscopic_compliment_checking(robot_arm_address, function_name_address) == 'y')
	{
		(*robot_arm_address).ractangular_macro_compliment_value = 'n';
		(*robot_arm_address).stream_of_time = 0;
		
		(*robot_arm_address).position_value.macroscopic_destination_position_x = purpose_position_x;
		(*robot_arm_address).position_value.macroscopic_destination_position_y = purpose_position_y;
		(*robot_arm_address).position_value.macroscopic_destination_position_z = purpose_position_z;
		(*function_name_address).macroscopic_destination_position_seta = purpose_seta;
		(*function_name_address).macroscopic_destination_position_grip = purpose_grip;
		(*robot_arm_address).time_out = purpose_time_out;
	}
}
//<끝>========================================
//<시작>========================================
int function_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double seta, double grip)
{
	
}
//<끝>========================================
//<시작>========================================
int position_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double x, double y, double z)
{
	
}
//<끝>========================================
//<시작>========================================
void combine_macroscopic_to_micro_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{

	(*robot_arm_address).position_value.microscopic_destination_position_x =
	(*robot_arm_address).position_value.macroscopic_current_position_x + ((*robot_arm_address).position_value.macroscopic_destination_position_x - (*robot_arm_address).position_value.macroscopic_current_position_x) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_y =
	(*robot_arm_address).position_value.macroscopic_current_position_y + ((*robot_arm_address).position_value.macroscopic_destination_position_y - (*robot_arm_address).position_value.macroscopic_current_position_y) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_z =
	(*robot_arm_address).position_value.macroscopic_current_position_z + ((*robot_arm_address).position_value.macroscopic_destination_position_z - (*robot_arm_address).position_value.macroscopic_current_position_z) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	//요기의 부분 설정에 따라, function part의 모터 제어를 func 내부의 position 구조체를 이용할 지, motor_mathod를 이용할지 선택할 수 있음.
	
	
	(*function_name_address).microscopic_destination_position_seta = (*function_name_address).macroscopic_current_position_seta + ((*function_name_address).macroscopic_destination_position_seta - (*function_name_address).macroscopic_current_position_seta) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	(*function_name_address).microscopic_destination_position_grip = (*function_name_address).macroscopic_current_position_grip + ((*function_name_address).macroscopic_destination_position_grip - (*function_name_address).macroscopic_current_position_grip) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);

}

//<끝>========================================
//<시작>========================================
int function_part_position_transformation(double seta, double length, double x, double y, double z, double* address_of_x, double* address_of_y, double* address_of_z)
{
	//정관절인 경우만 적동하도록 제한 한다면.

	if((seta-M_PI_2)*(seta-M_PI_2)>M_PI_2*M_PI_2)//범위에 같다가포함되면 func_seta의 범위가  0,인 경우와 M_PI인 경우에 에러 가 발생함
	{
		return 0;//여기서 러턴 0이 발생하여 함수가 갑자기 종료됨. 특히나 처벗째에서 걸리는 경우 곤란??
	}
	// 현재 함수의 경우  DEGREE_OF_SETA 가 atan2(y, x)를 통해 결정되는 경우 이므로 SETA의 값이 중요 하지 않음.
	//이 아니리ㅏ role 각도가 있는 경우 이므로 atan2(y, x)와 개별 적인 변수 임.
	double atan_value = 0;
	if(x == 0, y == 0)
	{
		atan_value = 0;
	}
	else
	{
		atan_value = atan2(y, x);
	}

	double sin_value = 0;
	if(seta == M_PI)
	{
		sin_value =  0;
	}
	else
	{
		sin_value = sin(seta);
	}
	
	double cos_value = 0;
	if(seta - (long)(seta / M_PI) == M_PI_2)
	{
		cos_value = 1;
	}
	else
	{
		cos_value = cos(seta);
	}

	*address_of_x = x - length*sin_value*cos(atan_value);
	*address_of_y = y - length*sin_value*sin(atan_value);
	*address_of_z = z - length*cos(seta);
}
//<끝>========================================
//<시작>========================================
int robot_4aix_arm_degree_clac(struct motor_list* robot_arm_address, double length_1, double length_2, double length_3, 
double x_destination, double y_destination , double z_destination, double function_target_degree, double* m1_degree, double* m2_degree, double* m3_degree, double* m4_degree, double* cumulated_angle_position)
{

	double r_0 = sqrt(x_destination*x_destination + y_destination*y_destination + z_destination*z_destination);
	
	
	
	double yaw_0 = atan2(y_destination, x_destination);
	
	
	
	double pitch_0 = atan2(z_destination, sqrt(x_destination*x_destination + y_destination*y_destination));//sholder
	double phi_0 = M_PI_2-pitch_0;
	double side_1 = sqrt(length_1*length_1 + r_0*r_0 - 2*length_1*r_0*cos(phi_0));
	
	//계산 할 각도가 90인경우 근사적으로 +오차가 발생하여 asin의 범위를 벗어나는 오차가 있음. 따라서 1보다값이 더 큰 경우 보정이 필요함. 다행이도 sin의 치역이 -1~1까지라는 특성을 활용하여  보정가능. 
	//또는 제2 코사인 법칙을 이용하는경우에 오차가 발생하지 않는지 확인해보고 문제가 없으면 적용해 볼 수 있음. 
	
	/*
	x_destination, y_destination, z_destination이 0이되어,side_1이 0이 되는 경우에는 
	[r_0 / side_1 * sin(phi_0)]과 같은 구문에서 0으로 나누는 에러가 발생할 수 있다.
	그래도 이런 경우는 목표좌표가 0으로 접근하거나, 시작 좌표가 0에서 시작하는 경우이므로 계산 가능하다. 
	이런 경우는 현재좌표가 시점이고 목표 좌표가 종점이 되는 벡터를 구하고, 
	Z축이 높이의 방향일때 밑바닥에 해당하는 XY평면과 앞서과한 벡터사이의 평면과 직선 사이의 각도 관계를 통해 계산하면 된다.
	벡터의 시점이 (0, 0, length_1)에서 시작하는 경우로 계산 할 때와 종점이 (0, 0, length_1)인 경우로 계산할 때  
	평면과 벡터사이의 각도는 XY평면에 대하여 반전되어있는 형태를 가짐으로 계산에서 주의가 필요하다. 
	*/
	
	double pitch_1 = 0;
	double phi_1 = 0;
	if(
	((*robot_arm_address).position_value.transformed_current_position_x == 0 && (*robot_arm_address).position_value.transformed_current_position_y == 0 && (*robot_arm_address).position_value.transformed_current_position_z-length_1 == 0)
	|| ((*robot_arm_address).position_value.transformed_destination_position_x == 0 && (*robot_arm_address).position_value.transformed_destination_position_y == 0 && (*robot_arm_address).position_value.transformed_destination_position_z-length_1 == 0)
	)
	{
		double r_proj_xy_0 = sqrt((*robot_arm_address).position_value.transformed_current_position_x * (*robot_arm_address).position_value.transformed_current_position_x + (*robot_arm_address).position_value.transformed_current_position_y * (*robot_arm_address).position_value.transformed_current_position_y);
		double r_proj_xy_1 = sqrt((*robot_arm_address).position_value.transformed_destination_position_x * (*robot_arm_address).position_value.transformed_destination_position_x + (*robot_arm_address).position_value.transformed_destination_position_y * (*robot_arm_address).position_value.transformed_destination_position_y);
		
		double delta_r_proj_xy = r_proj_xy_1 - r_proj_xy_0;
		
		double delta_z = (*robot_arm_address).position_value.transformed_destination_position_z - (*robot_arm_address).position_value.transformed_current_position_z;
		
		if(((*robot_arm_address).position_value.transformed_current_position_x == 0 && (*robot_arm_address).position_value.transformed_current_position_y == 0 && (*robot_arm_address).position_value.transformed_current_position_z-length_1 == 0))
		{
			//시점의 좌표가 (0, 0, length_1)인 경우 
			/*
			높이 축과의 각도는, 기준이 되는 동경인 x축과 수직관계이므로 
			계산 되는 각도가 x축과 seta만큼의 각도를 이루고 해답이 pitch_1일때
			pitch_1 = 90 + seta;
			그리고 좌표와를 알고 있음으로 seta = atan2(y, x)로 대입하면 
			*/
			pitch_1 = M_PI_2 + atan2(delta_z, delta_r_proj_xy);
		}
		else
		{
			/*
			동경인 x축과 각도를 이루는 벡터의 위치 벡터가 시점이  (0, 0, length_1)인 경우와 서로 점대칭인 관계에 있음으로
			각 좌표에 (-)부호를 붙여 계산하면 옳은 pitch_1을 계산할 수 있을 것이다. 
			*/
			pitch_1 = M_PI_2 + atan2(-delta_z, -delta_r_proj_xy);
		}
		
		phi_1 = M_PI_2;
		
	}
	else
	{
		double check_value = r_0 / side_1 * sin(phi_0);
		if(check_value > 1)
		{
			check_value = 1;
		}
		pitch_1 = asin(check_value);
		phi_1 = acos((side_1*side_1 + length_2*length_2 - length_3*length_3)/(2*side_1*length_2));
	}
	
	
	double resultant_1 = M_PI - (phi_1 + pitch_1);
	double sigma = asin(side_1 / length_3 * sin(phi_1));
	double resultant_2 = M_PI - sigma;
	*cumulated_angle_position = -(resultant_1 + resultant_2);
	*m1_degree = yaw_0;
	*m2_degree = resultant_1;
	*m3_degree = resultant_2;
	*m4_degree = -(resultant_1 + resultant_2) + function_target_degree;
	
}
//<끝>========================================
//<시작>========================================
void degree_to_step(struct motor_list* robot_arm_address, int number_of_motors)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	double rad_per_step = 2*M_PI/(double)(*motor_Ptr).step_per_revolution_presison;
	
	int i =0;
	while(i<number_of_motors)
	{
		(*motor_Ptr).destination_step = (long)  (((*motor_Ptr).destination_degree / (2*M_PI))* (double)(*motor_Ptr).step_per_revolution_presison);
		motor_Ptr++;
		i = i + 1;
	}
}
//<끝>========================================
//<시작>========================================
void ordering_trans_form(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	function_part_position_transformation(
	(*function_name_address).microscopic_destination_position_seta, (*function_name_address).length,
	(*robot_arm_address).position_value.microscopic_destination_position_x, (*robot_arm_address).position_value.microscopic_destination_position_y, (*robot_arm_address).position_value.microscopic_destination_position_z,
	(&robot_arm_1.position_value.transformed_destination_position_x), (&robot_arm_1.position_value.transformed_destination_position_y), &(robot_arm_1.position_value.transformed_destination_position_z)
	);
	
	robot_4aix_arm_degree_clac(
	robot_arm_address,
	(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3,
	(*robot_arm_address).position_value.transformed_destination_position_x, (*robot_arm_address).position_value.transformed_destination_position_y, (*robot_arm_address).position_value.transformed_destination_position_z,
	(*function_name_address).microscopic_destination_position_seta, // 사용중인 변 수 인지 확인 해 보기.
	//m2_bldc_mode
	//&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	//m2_step_motor_mode
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_stepper.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	);
	
	(*robot_arm_address).m5_servo_gripper.destination_degree = (*function_name_address).microscopic_destination_position_grip;
	
	
	degree_to_step(&robot_arm_1, 5);
	
}
//<끝>========================================
//<시작>========================================
void rectangular_coordinates_microscopic_to_driver_ordering(struct motor_list* robot_arm_address)
{//모터 드리이버는 직교 좌표계의 형태로 움직이더라도 sin, cos tan 등 삼각 함수의 형태를 취하고 있음으로 무의미
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	while(n < 5)
	{
		(*motor_Ptr).direction_value = direction_value_clac_arg_2((*motor_Ptr).displacement_step, (*motor_Ptr).motor_driving_mode);
		motor_Ptr++;
		n = n + 1;
	}
}
//<끝>========================================


//<수직선형 각도 명령 부분>99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999
//<시작>========================================
void number_line_first_class_order(struct motor_mathod* motor_name, long position)
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

//<기계 입출력, 모터 제어 관련함수>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//<시작>========================================
void output_safety_function(char port_name ,char object_pin)//모터용 출력으로 사용되는 핀레지스터만 제어하는 기능 추가 필요.!!!!!!!!!!!!!!!!!!!!!!!!!
{
	port_output_assistant(port_name, object_pin, 0b0000000);
	_delay_us(5);
}
//<끝>==========================================
//<시작>========================================
int hall_sensor_reding(char port_name, char pin_data = 0b00000111)
{
	return  port_input_assistant(port_name, pin_data);
}
//<끝>==========================================
//<시작>========================================
void input_value_to_brige_open_loop(char port_name, char object_pin ,char binary_char = 0b00000000, int time_ms = 0)//a part of auto output system.
{
	port_output_assistant(port_name, object_pin, binary_char);
	delay_costom_ms(time_ms);//it should be changed, it can control several of motors in same time. doesn't motors are stopped by that delay function.
	output_safety_function(port_name, object_pin);
}
//<끝>==========================================
//<시작>========================================
void step_motor_driving_open_loop(char motor_port, char motor_pin, char* direction_value, long* current_step_address)
{
	char out_put_value = 0;
	char dir_forward_back = 0;
	char step_value = 0;
	char holding_value = 0;
	
	switch(*direction_value)
	{
		case 0:
			dir_forward_back = 0;
			step_value = 0;
			holding_value = 1;
			break;
		case 1: // forward
			dir_forward_back = 1;
			step_value = 1;
			holding_value = 0;//enable 이 1인 경우에는 정지 명령을 의미함.
			//printf("forward ++");
			break;
		case 2: // backward
			dir_forward_back = 0;
			step_value = 1;
			holding_value = 0;
			//printf("backward--");
			break;
		case 3:
			dir_forward_back = 0;
			step_value = 0;
			holding_value = 0;//enable = 0 : holding.
			break;
	}
	
	if(step_value == 1)
	{
		out_put_value |= (dir_forward_back<<2)|(step_value<<1)|(holding_value);
		port_output_assistant(motor_port, motor_pin, out_put_value);
		_delay_us(1000);
		out_put_value = 0;
		out_put_value |= (dir_forward_back<<2)|(0<<1)|(holding_value);
		port_output_assistant(motor_port, motor_pin, out_put_value);
		_delay_us(1000);
	}
	//output_safety_function(motor_port, motor_pin);
	
	virtual_position_management(current_step_address, *direction_value);
	//current_position_value 증감시키기.
}
//<끝>==========================================
//<시작>========================================
void step_motor_power_saving(struct motor_list* robot_arm_address, int number_of_motor)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	int n = 0;
	
	while(n < number_of_motor)
	{
		if((*motor_Ptr).motor_type == 's' || (*motor_Ptr).motor_type == 'S')
		{
			port_output_assistant((*motor_Ptr).motor_port, (*motor_Ptr).motor_pin, 0b001);
		}
		motor_Ptr++;
		n = n + 1;
	}
}
//<끝>==========================================
//<시작>========================================
void bldc_motor_driving_open_loop(char port_name, int object_pin, char direction_value,  int* current_hall_sensor_value_address, long* current_step_address)
{
	char output_signal = 0;
	virtual_hall_value_generator(current_hall_sensor_value_address,*current_step_address,direction_value);
	output_signal = sign_calculating(*current_hall_sensor_value_address, direction_value);
	//printf("체크 7 : %d\n", output_signal);
	input_value_to_brige_open_loop(port_name, object_pin, output_signal, 50);//anqngk tkdxodptj 20 ms 동안 5v내외의 전압으로 탈조 없이 진행 되었음.... 그러나 부하가 있는 상태에서는 상태에 따라 필요한 전력 공급 시간이 다를 수 있으니 모터의 상태에 따른 피드백 제어가 필요함... 증 스텝이 1(or -1)증가 할때까지 신호를 공급해야 하는 것임.
	virtual_position_management(current_step_address, direction_value);
	
}
//<끝>========================================
//<시작>======================================
void bldc_motor_driving_closed_loop(char port_name, int motor_pin, int* current_hall_sensor_value_address, int* just_before_hall_sensor_value_address, char* direction_value_address)
{
	while(bit_analysis(*just_before_hall_sensor_value_address, *current_hall_sensor_value_address) == 0)
	{
		port_output_assistant(port_name, motor_pin, sign_calculating(*current_hall_sensor_value_address, *direction_value_address));
	}
	//output_safety_function(port_name, motor_pin);?딜래이가 생각 이상으로 길어진 탓에 제어에 문제가 생긴다.
}
//<끝>========================================
//<시작>======================================
void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value_address, int* just_before_hall_sensor_value_address, char* direction_value_address, long* current_step_address)//, char hall_sensor_port, int hall_sensor_pin 등 홀센서 값읽기는 실시간 감시가 중요하기에 타이머 인터럽트가 대신 담당한다.
{
	//이 함수에서 스텝모터/bldc, 강제 홀딩 여부, 포트및 핀설정, 방향 , 모든 것을 판단하여 말단 함수들에게 명령을내림.
	
	switch(motor_type)
	{
		case 'b':
		case 'B':
		bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value_address, just_before_hall_sensor_value_address, direction_value_address);//파라메터를 받은 순간의 값과 함수가 실행중일 때의 값이 서로 다를 수 있기때문에 타이머 인터럽트로 계속 초기화 하는 값을 메모리 주소와 포인터를 이용하여 전달한다.
		break;
		case 's':
		case 'S':
		step_motor_driving_open_loop(motor_port, motor_pin, direction_value_address, current_step_address);//딜레이 타임도 넣어러주면 섬세한 모터 제어에 좋을듯. 단 구조체에 미리 저장해두어 모터의 속성을 가져다 쓰듯이 사용하기.
		break;
		case 'v':
		case 'V':
		virtual_position_management(current_step_address, *direction_value_address);
		break;
	}
}

//<끝>========================================
//<시작>========================================
void motor_combo_serise_drivig(struct motor_list* robot_arm_address, int number_of_motor)
{
	
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	int n = 0;
	
	while(n < number_of_motor)
	{
		
		if((*motor_Ptr).microscopic_complition == 'n')
		{
			
			general_motor_drivig((*motor_Ptr).motor_type, (*motor_Ptr).motor_port, (*motor_Ptr).motor_pin, &((*motor_Ptr).current_hall_sensor_value), &((*motor_Ptr).just_before_hall_sensor_value), &((*motor_Ptr).direction_value), &((*motor_Ptr).current_step));
		}
		motor_Ptr++;
		n = n + 1;
	}
}
//<끝>========================================

//<동작완료 점검 함수>BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
//<시작>========================================
int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{

	//printf("%c\n", (*robot_arm_address).ractangular_macro_compliment_value);
	if((*robot_arm_address).ractangular_macro_compliment_value == 'n')
	{
		if(((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out) > 1)
		{
			(*robot_arm_address).position_value.macroscopic_current_position_x = (*robot_arm_address).position_value.microscopic_current_position_x;
			(*robot_arm_address).position_value.macroscopic_current_position_y = (*robot_arm_address).position_value.microscopic_current_position_y;
			(*robot_arm_address).position_value.macroscopic_current_position_z = (*robot_arm_address).position_value.microscopic_current_position_z;
			
			(*function_name_address).macroscopic_current_position_seta = (*function_name_address).microscopic_current_position_seta;
			(*function_name_address).macroscopic_current_position_grip = (*function_name_address).microscopic_current_position_grip;
			
			(*robot_arm_address).ractangular_macro_compliment_value = 'y';
			return 'y';
		}
		else
		{
			return 'n';
		}
	}
	if((*robot_arm_address).ractangular_macro_compliment_value == 'y')
	{
		//time_initialze
		(*robot_arm_address).stream_of_time = 0;
		return 'y';
	}
}

//<끝>========================================
//<시작>========================================
int rectangular_coordinate_microscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	//미시적 단계에서 어떻게 해결할까.....? .스텝 단위에서  현제위치와 완료 상태를 확인 하여야 할 듯 함.
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	while(n < 5)
	{

		(*motor_Ptr).displacement_step = ((*motor_Ptr).destination_step - (*motor_Ptr).current_step);
		if((*motor_Ptr).displacement_step != 0)
		{
			(*motor_Ptr).microscopic_complition = 'n';
			//이부분은 micro to diriver 부분으로 옮겨 야 할 듯. 
			m = m + 1;
		}
		else
		{
			(*motor_Ptr).current_degree = (*motor_Ptr).destination_degree;
			(*motor_Ptr).microscopic_complition = 'y';
		}
		motor_Ptr++;
		n = n + 1;
	}
	if(m != 0)
	{
		return 'n';
	}
	else
	{
		(*robot_arm_address).position_value.microscopic_current_position_x = (*robot_arm_address).position_value.microscopic_destination_position_x;
		(*robot_arm_address).position_value.microscopic_current_position_y = (*robot_arm_address).position_value.microscopic_destination_position_y;
		(*robot_arm_address).position_value.microscopic_current_position_z = (*robot_arm_address).position_value.microscopic_destination_position_z;
		/*
		(*robot_arm_address).position_value.transformed_current_position_x = (*robot_arm_address).position_value.transformed_destination_position_x;
		(*robot_arm_address).position_value.transformed_current_position_y = (*robot_arm_address).position_value.transformed_destination_position_y;
		(*robot_arm_address).position_value.transformed_current_position_z = (*robot_arm_address).position_value.transformed_destination_position_z;
		*/	
		(*function_name_address).microscopic_current_position_seta = (*function_name_address).microscopic_destination_position_seta;
		(*function_name_address).microscopic_current_position_grip = (*function_name_address).microscopic_destination_position_grip;
		return 'y';
	}
}
//<끝>========================================
//<시작>========================================
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
		if(((*motor_Ptr).microscopic_current_position - (*motor_Ptr).microscopic_destination_position) != 0)
		{
			(*motor_Ptr).microscopic_complition = -1;
			(*motor_Ptr).microscopic_displacement_value = (*motor_Ptr).microscopic_destination_position - (*motor_Ptr).microscopic_current_position;
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

//<위치 초기화>CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//<시작>========================================
void motor_combo_calibration(struct motor_list* robot_arm_address, struct function_part* function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, int number_of_motor)
{
	//	combine_ordering(robot_arm_address, function_name_address, purpose_position_x, purpose_position_y, purpose_position_z, purpose_seta, purpose_grip, 1);
	//	(*robot_arm_address).stream_of_time = 1;
	//	ordering_trans_form(robot_arm_address, function_name_address);

	function_part_position_transformation(purpose_seta, (*function_name_address).length,
	purpose_position_x, purpose_position_y, purpose_position_z,
	&(*robot_arm_address).position_value.transformed_current_position_x, &(*robot_arm_address).position_value.transformed_current_position_y, &(*robot_arm_address).position_value.transformed_current_position_z
	);
	
	robot_4aix_arm_degree_clac(
	robot_arm_address,
	(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3,
	(*robot_arm_address).position_value.transformed_current_position_x, (*robot_arm_address).position_value.transformed_current_position_y, (*robot_arm_address).position_value.transformed_current_position_z,
	purpose_seta, // 사용중인 변 수 인지 확인 해 보기.
	//m2_bldc_mode
	//&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	//m2_step_motor_mode
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_stepper.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	);
	
	degree_to_step(&robot_arm_1, 5);
	
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	int n = 0;
	
	while(n < number_of_motor)
	{
		(*motor_Ptr).current_step = (*motor_Ptr).destination_step;
		motor_Ptr++;
		n = n + 1;
	}
	
	//current_position_organizing.
	
	(*robot_arm_address).position_value.macroscopic_current_position_x = purpose_position_x;
	(*robot_arm_address).position_value.macroscopic_current_position_y = purpose_position_y;
	(*robot_arm_address).position_value.macroscopic_current_position_z = purpose_position_z;
	(*function_name_address).macroscopic_current_position_seta = purpose_seta;
	(*robot_arm_address).m5_servo_gripper.destination_degree = purpose_grip;
	(*robot_arm_address).stream_of_time = 0;

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
			number_line_first_class_order(&robot_arm_1.m2_bldc, 200);
		}
		else
		{
			number_line_first_class_order(&robot_arm_1.m2_bldc, 0);
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


//<총 구동 함수>DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
void whole_driving()
{
	while(rectangular_coordinate_macroscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
	{
		combine_macroscopic_to_micro_ordering(&robot_arm_1, &func_gripper);
		ordering_trans_form(&robot_arm_1, &func_gripper);
		while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			//PORTD |= 0b00000001;
			motor_combo_serise_drivig(&robot_arm_1, 5);
			//PORTD |= 0b00000010;
		}
		//step_motor_power_saving(&robot_arm_1, 5);//2020.02.29 15:45 신규 추가.
		//microscopic_current_position_clac(&robot_arm_1, 4);
		//PORTD = 0b00000000;
		
		robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
	}
}


//<타이머 비교 일치 인터럽트>EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
//<시작>========================================
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
//<끝>========================================
//<시작>========================================
//timer ISR part <begin>
/*
ISR(TIMER0_COMP_vect)//타이머 인터럽트 부분, 홀센서 값 읽기 스텝수 누계 계산 등
{
	UART1_init();
	//hall_sensor reading and position management
	//<시작>========================================
	//이부분은 인터럼트가 실행된이후 동작하는 부분.//일단 타이머 인터럽트를 이용합시다.그리고 '속도 제한'을 둡시다. 매 타임아웃마다 모든 엔코더의 스텝 변화값을 기록하고 누계에 넣읍시다.
	//encoder_management(&robot_arm_1.m2_bldc);
	
	//UART1_transmit(robot_arm_1.m2_bldc.cumulated_steps);
	UART1_transmit('e');
	//int* motor_list, int* motor_name,
	//return consumed time






	//<끝>==========================================
}
*/
//<끝>========================================


//<주 명령문>FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
int main(void)
{
	
	
	//UART1_init();
	//<시작>변수 초기값 설정=========================================
	//DDRA = 0b11111111;
	DDRD = 0b11111111;
	DDRB = 0b11111111;
	//PORTB = 0b11111111;
	//<시작>m1 초기값 설정=========================================
	robot_arm_1.m1_stepper.motor_type = 's';
	robot_arm_1.m1_stepper.motor_driving_mode = 'n';
	robot_arm_1.m1_stepper.step_per_revolution_presison = 21525;//1435*3*5
	robot_arm_1.m1_stepper.proper_direction = 1;
	
	robot_arm_1.m1_stepper.motor_port = 'd';
	robot_arm_1.m1_stepper.motor_pin = 0b00000111;
	//robot_arm_1.m1_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m1_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m1_stepper.just_before_hall_sensor_value = -1;

	//<끝>m1 초기값 설정=========================================
	
	//<시작>m2 초기값 설정=========================================
	robot_arm_1.m2_stepper.motor_type = 's';
	robot_arm_1.m2_stepper.motor_driving_mode = 'n';
	robot_arm_1.m2_stepper.step_per_revolution_presison = 645750;//1435*3*3*5
	robot_arm_1.m2_stepper.proper_direction = 1;
	
	robot_arm_1.m2_stepper.motor_port = 'd';
	robot_arm_1.m2_stepper.motor_pin = 0b00111000;
	//robot_arm_1.m2_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m2_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m2_stepper.just_before_hall_sensor_value = -1;

	/*
	//<시작>m2_bldc_mode=========================================
	robot_arm_1.m2_bldc.motor_type = 'b';
	robot_arm_1.m2_bldc.motor_driving_mode = 'n';
	robot_arm_1.m2_bldc.step_per_revolution_presison = 100;;
	robot_arm_1.m2_bldc.proper_direction = 1;
	
	robot_arm_1.m2_bldc.motor_port = 'b';
	robot_arm_1.m2_bldc.motor_pin = 0b00111111;
	robot_arm_1.m2_bldc.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m2_bldc.current_hall_sensor_value = -1;
	robot_arm_1.m2_bldc.just_before_hall_sensor_value = -1;
	//<끝>m2_bldc_mode=========================================
	*/
	//<끝>m2 초기값 설정=========================================
	//<시작>m3 초기값 설정=========================================
	robot_arm_1.m3_stepper.motor_type = 's';
	robot_arm_1.m3_stepper.motor_driving_mode = 'n';
	robot_arm_1.m3_stepper.step_per_revolution_presison = 12915;//1435*3*3
	robot_arm_1.m3_stepper.proper_direction = 1;
	
	robot_arm_1.m3_stepper.motor_port = 'b';
	robot_arm_1.m3_stepper.motor_pin = 0b00111000;
	//robot_arm_1.m3_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m3_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m3_stepper.just_before_hall_sensor_value = -1;

	//<끝>m3 초기값 설정=========================================
	//<시작>m4 초기값 설정=========================================
	robot_arm_1.m4_stepper.motor_type = 's';
	robot_arm_1.m4_stepper.motor_driving_mode = 'n';
	robot_arm_1.m4_stepper.step_per_revolution_presison = 1435*1;
	robot_arm_1.m4_stepper.proper_direction = 1;
	
	robot_arm_1.m4_stepper.motor_port = 'b';
	robot_arm_1.m4_stepper.motor_pin = 0b00000111;
	//robot_arm_1.m4_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m4_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m4_stepper.just_before_hall_sensor_value = -1;

	//<끝>m4 초기값 설정=========================================
	//<시작>m5 초기값 설정=========================================
	robot_arm_1.m5_servo_gripper.motor_driving_mode = 'n';
	robot_arm_1.m5_servo_gripper.step_per_revolution_presison = 1;
	robot_arm_1.m5_servo_gripper.proper_direction = 1;
	
	robot_arm_1.m5_servo_gripper.motor_port = 'b';
	robot_arm_1.m5_servo_gripper.motor_pin = 0b01000000;
	//robot_arm_1.m5_servo_gripper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m5_servo_gripper.current_hall_sensor_value = -1;
	robot_arm_1.m5_servo_gripper.just_before_hall_sensor_value = -1;

	//<끝>m5 초기값 설정=========================================
	step_motor_power_saving(&robot_arm_1, 5);
	robot_arm_1.ractangular_macro_compliment_value = 'y';
	//<끝>변수 초기값 설정===========================================
	/*
	//<시작>타이머 인터럽트 조건 설정=================================
	TCCR0 = (1<<WGM00) | (1<<CS0);
	
	TCNT0 = 0;
	
	OCR0 = 10;
	
	TIMSK |= (1<<OCIE0);
	
	sei();
	//<끝>타이머 인터럽트 조건 설정===================================
	*/
	
	
	//current_position_caliberation(&robot_arm_1.m2_bldc);//용도가 무엇? = 모터캘리브 레이션 하위 호환. 현재 사용하지 않음.
	
	/*테스트 중 비활성화.
	//<시작>상태 초기화.=============================================
	motor_combo_calibration(&robot_arm_1, &func_gripper, 20, 0, 37, 0, 255, 5);//last argument is number_of_motors
	//<끝>상태 초기화.=============================================
	//<시작>위치 명령및 구동=======================================
	combine_ordering(&robot_arm_1, &func_gripper, 20, 0, 37, 2*M_PI, 255, 100);
	whole_driving();
	//<끝>위치 명령및 구동=========================================
	*/
	
	
	///*
	//<시작>스텝 모터 위치제어 테스트==============================================================
	_delay_ms(1000);

	motor_combo_calibration(&robot_arm_1, &func_gripper, 40, 0, 0, 0, 255, 5);//last argument is number_of_motors

	combine_ordering(&robot_arm_1, &func_gripper, 30, 0, 30, 0, 255, 1);

	whole_driving();
	
	//combine_ordering(&robot_arm_1, &func_gripper, 40, 0, 0, 0, 255, 1000);

	//whole_driving();

	//combine_ordering(&robot_arm_1, &func_gripper, 20, 0, 27, 0, 255, 100);
	//whole_driving();
	step_motor_power_saving(&robot_arm_1, 5);
	//<끝>스텝 모터 위치제어 테스트==============================================================
	//*/
	/*
	robot_arm_1.m1_stepper.current_degree
	robot_arm_1.m2_stepper.current_degree
	robot_arm_1.m3_stepper.current_degree
	robot_arm_1.m4_stepper.current_degree
	
	
	
	robot_arm_1.m1_stepper.current_step
	robot_arm_1.m2_stepper.current_step
	robot_arm_1.m3_stepper.current_step
	robot_arm_1.m4_stepper.current_step
	
	
	*/
	

	/*
	//<시작>각도기준 명령테스트==================================================(성공)
	_delay_ms(1000);
	//PORTD = 0b00000001;
	//int k = 0;
	//for(k = 0; k < 5;k = k+1)
	//{
	{
	
		
		robot_arm_1.m3_stepper.current_degree = 0;
		robot_arm_1.m3_stepper.destination_degree = M_PI;
		robot_arm_1.m4_stepper.current_degree = 0;
		robot_arm_1.m4_stepper.destination_degree = M_PI;
		degree_to_step(&robot_arm_1, 4);
		while (rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
		}
		step_motor_power_saving(&robot_arm_1, 5);
		
		robot_arm_1.m3_stepper.destination_degree = 0;
		robot_arm_1.m4_stepper.destination_degree = 0;
		degree_to_step(&robot_arm_1, 4);
		while (rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
		}
		step_motor_power_saving(&robot_arm_1, 5);
	//}

	//PORTD = 0b00000000;
	//<끝>각도기준 명령테스트==================================================
	*/
	
	
	/*
	//<끝>스텝 모터 테스트_1_========================================
	_delay_ms(1000);
	PORTD = 0b00000001;
	
	
	robot_arm_1.m1_stepper.current_step = 0;
	robot_arm_1.m1_stepper.destination_step = 1000;
	while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
	{
		rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
		motor_combo_serise_drivig(&robot_arm_1, 5);
	}
	PORTD = 0b00000000;
	PORTD = 0b00000010;
	//robot_arm_1.m1_stepper.current_step = 1000;
	//robot_arm_1.ractangular_macro_compliment_value = 'y';
	robot_arm_1.m1_stepper.destination_step = 0;
	while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
	{
		rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
		motor_combo_serise_drivig(&robot_arm_1, 5);
	}
	
	step_motor_power_saving(&robot_arm_1, 5);

	//PORTB = 0b11111111;
	PORTD = 0b00000000;
	//<끝>========================================

	*/
	return 0;
}
//<끝>========================================



