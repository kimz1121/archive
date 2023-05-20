/*
 * whole_motor_control.c
 *
 * Created: 2020-02-07 오후 5:34:43
 * Author :  iksw
 */ 

/*
signal code list

0 : natural
1 : forward
2 : backward
3 : stop

101, 'e' : error

0xff, ob11111111 : waitng,(will be fine soon)

*/


#include <avr/io.h>
#include <util/delay.h>
//<시작>========================================
struct motor_list
{
	struct motor_mathod
	{
		//바뀌기 어려운 기계고유적 특성 
		int motor_type;//'s' step,'b' bldc
		int presison;
		
		//기계고유적 특성과 사용자의 임의 지정의 사이
		char proper_direction;
		
		//사용자가 임의로 설치한 특성
		char motor_port;
		int motor_pin;
		int hall_sensor_pin;
		
		//기계부품 제어 특성
		char motor_driving_mode;//stop or natural.
		
		//하드웨어적 특성을 실시간으로 저장하기 위한 변수
		//general, 공통의
		int cumulated_steps;
		//hall_sensor_only, 홀센서 전용
		int current_hall_sensor_value;// = -1
		int just_before_hall_sensor_value;// = -1
		
		char motor_own_bit_new;// = -1
		char motor_own_bit_old;// = -1
		
		//프로그램적 요인으로 모터 별로 빈번하게 활용되는 변수로 구조체에 구속 시켰음.
		
		int displacement_value;
		int direction_value;
		
		double progress_situation;// = 0//진행상황
		char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		
		long macroscopic_destination_position;// = 0//거시적 경로 설정 함수에서 활용
		long macroscopic_current_position;// = 0
		
		long microscopic_destination_position;// = 0//미시적 경로 설정 함수에서 사용
		long microscopic_current_position;// = 0
		
	};
	struct motor_mathod m1_stepper;//제 1관절 모터
	struct motor_mathod m2_bldc;//제 2관절 모터
	struct motor_mathod m3_stepper;//제 3관절 모터
	struct motor_mathod m4_stepper;//제 4관절 모터
	void* ending_NULL_pointer;// = NULL
	
	long current_position_x;
	long current_position_y;
	long current_position_z;
	
	long destination_position_x;
	long destination_position_y;
	long destination_position_z;
	
	int munber_of_motors;
	
}robot_arm_1;

//<끝>==========================================




//<시작>========================================
//timer ISR part <begin> 
int ISR()//타이머 인터럽트 부분, 홀센서 값 읽기 스텝수 누계 계산 등
{
	/*
	sicuential_bit_input(hall_sensor_reding(pin_data), int* bit_new, int* bit_old);
	//<시작>========================================
	//이부분은 인터럼트가 실행된이후 동작하는 부분.//일단 타이머 인터럽트를 이용합시다.그리고 '속도 제한'을 둡시다. 매 타임아웃마다 모든 엔코더의 스텝 변화값을 기록하고 누계에 넣읍시다.
	int realtime_hall_sensor_dectecting(int pin_data, int* bit_new_s_adress, int* bit_old_s_adress)
	{
		//must use it with pin change interrupt
		sicuential_bit_input(hall_sensor_reding(pin_data), int* bit_new, int* bit_old);
		
	}

	//int* motor_list, int* motor_name,

	//return consumed time






	//<끝>==========================================
	*/
}
//<끝>========================================

//통신부 만들기.






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
	if(*bit_new_s_adress == bit_input)
	{
		return 0;
	}
	
	*bit_old_s_adress = *bit_new_s_adress;
	*bit_new_s_adress = bit_input;
	return 1;
}
//<끝>==========================================
//<시작>========================================
int object_binary_making(int object_pin, int binary_int)
{
	int result_binary = -1;
	int not_binary = ~object_pin;
	result_binary |= (object_pin & binary_int);
	result_binary &= (not_binary || binary_int);
	return result_binary;
}
//<끝>==========================================
int binary_fit_modifing(int object_pin, int binary_int, int object_bin_length, int )
{
	
}



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
void port_input_assistant(char port_name, int object_pin)
{
	int temp = -1;
	int result = -1;
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
		if(((pin>>i)%0b10))
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
}
//<끝>==========================================










//기계 분석 관련 함수
//<시작>========================================
char bit_analysis(int bit_before = 0b000, int bit_after = 0b000)//it return 0, 2, 3, or e. 0 means it isn't moved, 2 means forward, 3 means backward, e means error.
{
	//(bit>>2) + ((bit%0b100)>>1) + (bit%0b10), (bit>>2), ((bit%0b100)>>1), (bit%0b10) //자리수 합산 코드...
	char bit_u_bef = (bit_before>>2);
	char bit_v_bef = ((bit_before%0b100)>>1);
	char bit_w_bef = (bit_before%0b10);
	
	char bit_u_aft = (bit_after>>2);
	char bit_v_aft = ((bit_after%0b100)>>1);
	char bit_w_aft = (bit_after%0b10);

	
	if(find_coupled_nuber(bit_before) == 'e' || find_coupled_nuber(bit_after) == 'e' )
	{
		//printf("an error occurred, the return sign from a function \'find_coupled_nuber(int bit)\' wasn't good.\n");
		return 'e';
	}
	
	char discriminant =
	((find_coupled_nuber(bit_before)^bit_u_bef)&&(1^(bit_u_bef^bit_u_aft)))*((find_coupled_nuber(bit_before)^bit_v_aft)*(2)+(find_coupled_nuber(bit_before)^bit_w_aft)*(3))
	+((find_coupled_nuber(bit_before)^bit_v_bef)&&(1^(bit_v_bef^bit_v_aft)))*((find_coupled_nuber(bit_before)^bit_w_aft)*(2)+(find_coupled_nuber(bit_before)^bit_u_aft)*(3))
	+((find_coupled_nuber(bit_before)^bit_w_bef)&&(1^(bit_w_bef^bit_w_aft)))*((find_coupled_nuber(bit_before)^bit_u_aft)*(2)+(find_coupled_nuber(bit_before)^bit_v_aft)*(3))
	//+6*((bit_u_bef^bit_u_aft)||(bit_v_bef^bit_v_aft)||(bit_w_bef^bit_w_aft))*(find_coupled_nuber(bit_before)^bit_u_bef)*(find_coupled_nuber(bit_before)^bit_v_bef)*(find_coupled_nuber(bit_before)^bit_w_bef)
	+(((find_coupled_nuber(bit_before)^bit_u_bef)&&(bit_u_bef^bit_u_aft))+((find_coupled_nuber(bit_before)^bit_v_bef)&&(bit_v_bef^bit_v_aft))+((find_coupled_nuber(bit_before)^bit_w_bef)&&(bit_w_bef^bit_w_aft)))*6
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
	int direction_value;
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
}
//<끝>==========================================
//<시작>========================================
int direction_value_clac_arg_2(int displacement, int driving_mode)
{
	int direction_value;
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
}
//<끝>==========================================
//<시작>========================================
long distance_calc_between_positions(long begin_point, long last_point)
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
int step_motor_driving_open_loop(int motor_port, int motor_pin, int direction_value)
{
	port_output_assistant(motor_port, motor_pin, direction_value)
	//current_position_value 증감시키기.
}
//<끝>==========================================
//<시작>========================================
int bldc_motor_driving_open_loop(char port_name, int* hall_sensor_value, int diretion_value)
{
	int output_signal = 0;
	output_signal = sign_calculating(*hall_sensor_value, diretion_value);
	input_value_to_brige_open_loop(port_name, output_signal, 20);//anqngk tkdxodptj 20 ms 동안 5v내외의 전압으로 탈조 없이 진행 되었음.... 그러나 부하가 있는 상태에서는 상태에 따라 필요한 전력 공급 시간이 다를 수 있으니 모터의 상태에 따른 피드백 제어가 필요함... 증 스텝이 1(or -1)증가 할때까지 신호를 공급해야 하는 것임.
	
	/*
	//오픈 루프 제어시 완료이후 현재 스텝 누계를 증감시키는 함수 가 필요함
	int open_loop_cumulating(int* current_step, int direction = 0, int variance = 0)
	{

		switch(direction)
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
		
		current_step 
	}
	*/
}
//<끝>========================================
//<시작>========================================
int bldc_motor_driving_closed_loop(char port_name, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int diretion_value)
{
	while(bit_analysis(*just_before_hall_sensor_value, *current_hall_sensor_value) == 0)
	{
		port_output_assistant(port_name, motor_pin, sign_calculating(*current_hall_sensor_value, diretion_value));
	}
	output_safety_function(port_name, motor_pin);
}
//<끝>========================================
//<시작>========================================
int general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int direction_value)//, char hall_sensor_port, int hall_sensor_pin 등 홀센서 값읽기는 실시간 감시가 중요하기에 타이머 인터럽트가 대신 담당한다. 
{
	//이 함수에서 스텝모터/bldc, 강제 홀딩 여부, 포트및 핀설정, 방향 , 모든 것을 판단하여 말단 함수들에게 명령을내림.
	switch(motor_type)
	{
		case 'b':
		case 'B':
			bldc_motor_driving_closed_loop(motor_port, motor_pin, *current_hall_sensor_value, *just_before_hall_sensor_value, direction_value)//파라메터를 받은 순간의 값과 함수가 실행중일 때의 값이 서로 다를 수 있기때문에 타이머 인터럽트로 계속 초기화 하는 값을 메모리 주소와 포인터를 이용하여 전달한다.
			break;
		case 's':
		case 'S':
			step_motor_driving_open_loop(motor_port, motor_pin, direction_value)//딜레이 타임도 넣어러주면 선세한 모터 제어에 좋을듯. 단 구조체에 미리 저장해두어 모터의 속성을 가져다 쓰듯이 사용하기. 
			break;
	}
}
//<끝>========================================
//<시작>========================================
int motor_combo_serise_drivig(struct motor_list* robot_arm_adress, int number_of_motor)
{
	n = 0;
	struct motor_list.motor_mathod* motor_Prt = robot_arm_adress;
	while(n < 4)
	{
		if(*(motor_Prt).direction_and_distance_value != 0)
		{
			general_motor_drivig(*(motor_Prt).motor_type, *(motor_Prt).motor_port, *(motor_Prt).motor_pin, &(*(motor_Prt).current_hall_sensor_value), &(*(motor_Prt).just_before_hall_sensor_value), )
		}
		else
		{
			*(motor_Prt).
		}
		motor_Prt+1;
		n = n + 1;
	}
	
}
//<끝>========================================





//경로설정 관련 함수.
//<시작>========================================
int macroscopic_ordering()
{
	
}
//<끝>========================================
//<시작>========================================
int microscopic_ordering(int motor_pin, int* hall_sensor_value, int steps, int speed)
{
	
}
//<끝>========================================


int macroscopic_compliment_checking(void* robot_arm_adress)//새로운 경로를 받기전에 경로가 완료 되었는지 확인하기 위해 쓰임.//이곳에서 경로와의 거리차가 +인지 -인지도 계산하기
{
	n = 0;
	m = 0;
	struct motor_list.motor_mathod* motor_Prt = robot_arm_adress;
	while(n < 4)
	{
		if(*(motor_Prt).macroscopic_current_position =! *(motor_Prt).macroscopic_destination_position)
		{
			*(motor_Prt).macroscopic_complition = -1;
			
			m = m + 1;
		}
		else
		{
			*(motor_Prt).macroscopic_complition = 1;
		}
		motor_Prt+1;
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
int microscopic_compliment_checking(void* robot_arm_adress)//새로운 경로를 받기전에 경로가 완료 되었는지 확인하기 위해 쓰임.
{
	n = 0;
	m = 0;
	struct motor_list.motor_mathod* motor_Prt = robot_arm_adress;
	while(n < 4)
	{
		if(distance_calc_between_positions(*(motor_Prt).microscopic_current_position, *(motor_Prt).microscopic_destination_position) != 0)
		{
			*(motor_Prt).microscopic_complition = -1;
			*(motor_Prt).direction_and_distance_value == distance_calc_between_positions(*(motor_Prt).microscopic_current_position, *(motor_Prt).microscopic_destination_position);
			m = m + 1;
		}
		else
		{
			*(motor_Prt).microscopic_complition = 1;
		}
		motor_Prt+1;
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
int main(void)
{
    /* Replace with your application code */
	while (1) 
	{
		

	}
}
//<끝>========================================
