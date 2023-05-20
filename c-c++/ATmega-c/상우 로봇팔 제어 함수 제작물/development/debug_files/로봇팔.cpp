#include<stdio.h>

int diradadadsdadafafafsfafsf;


void _delay_ms(int value)
{
 	 
}


#define PINA 0;
#define PINB 0;
#define PINC 0;
#define PIND 0;
#define PINE 0;
#define PINF 0;

char PORTA = 0;
char PORTB = 0;
char PORTC = 0;
char PORTD = 0;
char PORTE = 0;
char PORTF = 0;

/*
 * whole_motor_control.c
 *
 * Created: 2020-02-07 ���� 5:34:43
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


//#include <avr/io.h>
//#include <util/delay.h>
//<����>========================================

struct motor_mathod
	{
		//�ٲ�� ����� �������� Ư�� 
		int motor_type;//'s' step,'b' bldc
		int presison;
		
		//�������� Ư���� ������� ���� ������ ����
		char proper_direction;
		char  initialization_value;
		
		//����ڰ� ���Ƿ� ��ġ�� Ư��
		char motor_port;
		int motor_pin;
		int hall_sensor_pin;
		
		//����ǰ ���� Ư��
		char motor_driving_mode;//stop or natural.
		
		//�ϵ������ Ư���� �ǽð����� �����ϱ� ���� ����
		//general, ������
		int cumulated_steps;
		//hall_sensor_only, Ȧ���� ����
		int current_hall_sensor_value;// = -1
		int just_before_hall_sensor_value;// = -1
		
		char motor_own_bit_new;// = -1
		char motor_own_bit_old;// = -1
		
		//���α׷��� �������� ���� ���� ����ϰ� Ȱ��Ǵ� ������ ����ü�� ���� ������.
		
		int microscopic_displacement_value;
		int direction_value;
		
		double progress_situation;// = 0//�����Ȳ
		char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
		
		long macroscopic_destination_position;// = 0//�Ž��� ��� ���� �Լ����� Ȱ��
		long macroscopic_current_position;// = 0
		
		long microscopic_destination_position;// = 0//�̽��� ��� ���� �Լ����� ���
		long microscopic_current_position;// = 0
		
	};
struct motor_list
{
	
	struct motor_mathod m1_stepper;//�� 1���� ����
	struct motor_mathod m2_bldc;//�� 2���� ����
	struct motor_mathod m3_stepper;//�� 3���� ����
	struct motor_mathod m4_stepper;//�� 4���� ����
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

//<��>==========================================
//<����>========================================
//timer ISR part <begin> 
int ISR()//Ÿ�̸� ���ͷ�Ʈ �κ�, Ȧ���� �� �б� ���ܼ� ���� ��� ��
{
	/*
	sicuential_bit_input(hall_sensor_reding(pin_data), int* bit_new, int* bit_old);
	//<����>========================================
	//�̺κ��� ���ͷ�Ʈ�� ��������� �����ϴ� �κ�.//�ϴ� Ÿ�̸� ���ͷ�Ʈ�� �̿��սô�.�׸��� '�ӵ� ����'�� �ӽô�. �� Ÿ�Ӿƿ����� ��� ���ڴ��� ���� ��ȭ���� ����ϰ� ���迡 �����ô�.
	int realtime_hall_sensor_dectecting(int pin_data, int* bit_new_s_adress, int* bit_old_s_adress)
	{
		//must use it with pin change interrupt
		sicuential_bit_input(hall_sensor_reding(pin_data), int* bit_new, int* bit_old);
		
	}

	//int* motor_list, int* motor_name,

	//return consumed time






	//<��>==========================================
	*/
}
//<��>========================================

//��ź� �����.
//�⺻ �м� �Լ�
//<����>========================================
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
//<��>==========================================
//<����>========================================
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress)//���� ���縦 ���ؼ� �ּҸ� ����//it takes adress for deep copy
{
	//�ٲ��� ������쿡�� �Լ��� ����Ǿ� old�� new�� ���� ���� ���� �Ǿ� ������ ��Ȳ�� �����ϴ� ��� �߰�
	if(*bit_new_s_adress == -1)//�ʱ� �� �˻縦 -1�� �ϴ� ����������?
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
//<��>==========================================
//<����>========================================
int object_binary_making(int object_pin, int binary_int)
{
	int result_binary = 0;
	int not_binary = ~object_pin;
	result_binary |= (object_pin & binary_int);
	result_binary &= (not_binary | binary_int);
	return result_binary;
}
//<��>==========================================
/*
int binary_fit_modifing(int object_pin, int binary_int, int object_bin_length, int signal_binary_length)
{
	
}
*/
//�⺻ ���� �Լ�.
//<����>========================================
void delay_costom_ms(int time_ms = 0)
{
	int n = 0;
	while(n < time_ms)
	{
		_delay_ms(1);
		n=n+1;
	}
}
//<��>==========================================

//���� ����� ���� �Լ�
//<����>========================================
int port_input_assistant(char port_name, int object_pin)
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
//<��>==========================================
//<����>========================================
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
//<��>==========================================
//��� �м� ���� �Լ�
//<����>========================================
char bit_analysis(int bit_before = 0b000, int bit_after = 0b000)//it return 0, 2, 3, or e. 0 means it isn't moved, 2 means forward, 3 means backward, e means error.
{
	//(bit>>2) + ((bit%0b100)>>1) + (bit%0b10), (bit>>2), ((bit%0b100)>>1), (bit%0b10) //�ڸ��� �ջ� �ڵ�...
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
//<��>==========================================
//<����>========================================
//this function calculate hall sensor signals and than return output signals by situation of diretion_value.
int sign_calculating(int hall_sensor_value, char diretion_value)//int object_pin ����ϴ� ��� �߰��ϱ�.
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
//<��>==========================================
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
//<����>========================================
void virtual_position_management(long* current_position, int direction_value)//use it with dealy. 
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
//<��>==========================================
//<����>========================================
void current_position_caliberation(struct motor_mathod* motor_name)
{
	(*motor_name).microscopic_current_position = 0;
	(*motor_name).initialization_value = 1;
}
//<��>==========================================
/*
//<����>========================================
int motor_data_calculater(struct motor_mathod* motor_name, int pointer_shift_variance)
{
	(*(motor_name) + pointer_shift_variance)
}
//<��>==========================================
*/
//<����>========================================
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
//<��>==========================================
//<����>========================================
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
//<��>==========================================
//<����>========================================
long displacement_calc(long begin_point, long last_point)
{
	return last_point - begin_point;// last - begin is positive 
}
//<��>==========================================
//��� ����� �����Լ�
//<����>========================================
void output_safety_function(char port_name ,int object_pin)//���Ϳ� ������� ���Ǵ� �ɷ������͸� �����ϴ� ��� �߰� �ʿ�.!!!!!!!!!!!!!!!!!!!!!!!!!
{
	port_output_assistant(port_name, object_pin, 0b0000000);
	_delay_ms(5);
}
//<��>==========================================
//<����>========================================
int hall_sensor_reding(char port_name, int pin_data = 0b00000111)
{
	return  port_input_assistant(port_name, pin_data);
}
//<��>==========================================
//<����>========================================
void input_value_to_brige_open_loop(char port_name, int object_pin,int binary_int = 0b00000000, int time_ms = 0)//a part of auto output system.
{
	port_output_assistant(port_name, object_pin, binary_int);
	delay_costom_ms(time_ms);//it should be changed, it can control several of motors in same time. doesn't motors are stopped by that delay function.
	output_safety_function(port_name, object_pin);
}
//<��>==========================================
//<����>========================================
void step_motor_driving_open_loop(int motor_port, int motor_pin, int* direction_value)
{
	port_output_assistant(motor_port, motor_pin, *direction_value);
	//current_position_value ������Ű��.
}
//<��>==========================================
//<����>========================================
void bldc_motor_driving_open_loop(char port_name, int object_pin, int* hall_sensor_value, int diretion_value)
{
	int output_signal = 0;
	output_signal = sign_calculating(*hall_sensor_value, diretion_value);
	input_value_to_brige_open_loop(port_name, object_pin, output_signal, 50);//anqngk tkdxodptj 20 ms ���� 5v������ �������� Ż�� ���� ���� �Ǿ���.... �׷��� ���ϰ� �ִ� ���¿����� ���¿� ���� �ʿ��� ���� ���� �ð��� �ٸ� �� ������ ������ ���¿� ���� �ǵ�� ��� �ʿ���... �� ������ 1(or -1)���� �Ҷ����� ��ȣ�� �����ؾ� �ϴ� ����.
	
	/*
	//���� ���� ����� �Ϸ����� ���� ���� ���踦 ������Ű�� �Լ� �� �ʿ���
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
//<��>========================================
//<����>========================================
void bldc_motor_driving_closed_loop(char port_name, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* diretion_value)
{
	while(bit_analysis(*just_before_hall_sensor_value, *current_hall_sensor_value) == 0)
	{
		port_output_assistant(port_name, motor_pin, sign_calculating(*current_hall_sensor_value, *diretion_value));
	}
	output_safety_function(port_name, motor_pin);
}
//<��>========================================
//<����>========================================
void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* direction_value)//, char hall_sensor_port, int hall_sensor_pin �� Ȧ���� ���б�� �ǽð� ���ð� �߿��ϱ⿡ Ÿ�̸� ���ͷ�Ʈ�� ��� ����Ѵ�. 
{
	//�� �Լ����� ���ܸ���/bldc, ���� Ȧ�� ����, ��Ʈ�� �ɼ���, ���� , ��� ���� �Ǵ��Ͽ� ���� �Լ��鿡�� ���������.
	switch(motor_type)
	{
		case 'b':
		case 'B':
			bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value, just_before_hall_sensor_value, direction_value);//�Ķ���͸� ���� ������ ���� �Լ��� �������� ���� ���� ���� �ٸ� �� �ֱ⶧���� Ÿ�̸� ���ͷ�Ʈ�� ��� �ʱ�ȭ �ϴ� ���� �޸� �ּҿ� �����͸� �̿��Ͽ� �����Ѵ�.
			break;
		case 's':
		case 'S':
			step_motor_driving_open_loop(motor_port, motor_pin, direction_value);//������ Ÿ�ӵ� �־�ָ� ������ ���� ��� ������. �� ����ü�� �̸� �����صξ� ������ �Ӽ��� ������ ������ ����ϱ�. 
			break;
	}
}
//<��>========================================

//<����>========================================
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
//<��>========================================
//��μ��� ���� �Լ�.
//<����>========================================
int current_position_management()
{
	
}
//<��>========================================
//<����>========================================
int rectangular_coordinates_macroscopic_ordering()
{
	
}
//<��>========================================
//<����>========================================
int current_time_managemant()
{
	
}
//<��>========================================
//<����>========================================
int rectangular_coordinates_microscopic_ordering(int motor_pin, int* hall_sensor_value, int steps, int speed)
{
	
}
//<��>========================================
//<����>========================================
void first_class_order(struct motor_mathod* motor_name, long position)
{
	if((*motor_name).initialization_value == 1)
	{
		(*motor_name).macroscopic_destination_position = position;
		(*motor_name).macroscopic_current_position = (*motor_name).microscopic_current_position;
	}
}
//<��>========================================
//<����>========================================
int number_line_macroscopic_to_micro_ordering(struct motor_mathod* motor_name, double* current_time, double time_out)
{
	(*motor_name).microscopic_destination_position = (*motor_name).macroscopic_current_position + ((*motor_name).macroscopic_destination_position - (*motor_name).macroscopic_current_position)*(*current_time/time_out);
}
//<��>========================================
//<����>========================================
int number_line_microscopic_to_driver_ordering(struct motor_mathod* motor_name)
{
	(*motor_name).microscopic_displacement_value = (*motor_name).macroscopic_destination_position - (*motor_name).macroscopic_current_position;
	(*motor_name).direction_value = direction_value_clac_arg_2((*motor_name).microscopic_displacement_value, (*motor_name).motor_driving_mode);
}	
//<��>========================================

//���ۿϷ� ���� �Լ�
int macroscopic_compliment_checking(void* robot_arm_adress)//���ο� ��θ� �ޱ����� ��ΰ� �Ϸ� �Ǿ����� Ȯ���ϱ� ���� ����.//�̰����� ��ο��� �Ÿ����� +���� -������ ����ϱ�
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
//<��>========================================
//<����>========================================
int microscopic_compliment_checking(void* robot_arm_adress)//���ο� ��θ� �ޱ����� ��ΰ� �Ϸ� �Ǿ����� Ȯ���ϱ� ���� ����.
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
//<��>========================================
//<����>========================================
/*
int main(void)
{
	robot_arm_1.m2_bldc.motor_port = 'd';
	current_position_caliberation(&robot_arm_1.m2_bldc);

	while (1) 
	{
		current_position_caliberation(&robot_arm_1.m2_bldc);
		number_line_macroscopic_ordering(&robot_arm_1.m2_bldc, 30, robot_arm_1.stream_of_time, 30);
		while(microscopic_compliment_checking(&robot_arm_1) == -1)
		{
			number_line_macroscopic_ordering(&robot_arm_1.m2_bldc, 30, robot_arm_1.stream_of_time, 30);
			number_line_microscopic_ordering(&robot_arm_1.m2_bldc);//direction value ���Ϸ� ����.
			//microscopic_compliment_checking()
			virtual_hall_value_generator(&robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
			bldc_motor_driving_open_loop('d', 0b00111111, &robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.direction_value);
			virtual_position_management(&robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
		}
		robot_arm_1.stream_of_time =  robot_arm_1.stream_of_time + 1;
		if(robot_arm_1.stream_of_time > 60)
		{
			robot_arm_1.stream_of_time = 0;
		}
		_delay_ms(500);
	}
}
*/

int main(void)
{
	current_position_caliberation(&robot_arm_1.m2_bldc);
		int a = 0;
//		printf("��ġ �ʱ�ȭ\n"); 
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
//		printf("������ġ : %d\n", robot_arm_1.m2_bldc.macroscopic_current_position);
//		printf("üũ1 : %d\n", robot_arm_1.m2_bldc.macroscopic_destination_position);
//		printf("üũ2 : %d\n", macroscopic_compliment_checking(&robot_arm_1));
		
		while(macroscopic_compliment_checking(&robot_arm_1) == -1)
		{
			printf("������ġ : %d\n", robot_arm_1.m2_bldc.macroscopic_current_position);
			number_line_macroscopic_to_micro_ordering(&robot_arm_1.m2_bldc, &robot_arm_1.stream_of_time, 200);
			printf("��ǥ ��ġ : %d\n", robot_arm_1.m2_bldc.macroscopic_destination_position);
			printf("��ǥ ��ġ : %d\n", robot_arm_1.m2_bldc.microscopic_destination_position);
			while(microscopic_compliment_checking(&robot_arm_1) == -1)
			{
				
				number_line_microscopic_to_driver_ordering(&robot_arm_1.m2_bldc);//direction value ���Ϸ� ����.
				//microscopic_compliment_checking()
				virtual_hall_value_generator(&robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
				bldc_motor_driving_open_loop('D', 0b00111111, &robot_arm_1.m2_bldc.current_hall_sensor_value, robot_arm_1.m2_bldc.direction_value);
				virtual_position_management(&robot_arm_1.m2_bldc.microscopic_current_position, robot_arm_1.m2_bldc.direction_value);
	   			printf("üũ5 : %d\n", robot_arm_1.m2_bldc.microscopic_current_position);
	   			printf("üũ6 : %d\n", robot_arm_1.m2_bldc.direction_value);
			}
			robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
			_delay_ms(500);
		}
		
		printf("üũ4 : %d\n", robot_arm_1.stream_of_time);
//		if(robot_arm_1.stream_of_time > 60)
//		{
//			robot_arm_1.stream_of_time = 0;
//			printf("�ð� �ʱ�ȭ\n"); 
//		}
	}
}
//<��>========================================

