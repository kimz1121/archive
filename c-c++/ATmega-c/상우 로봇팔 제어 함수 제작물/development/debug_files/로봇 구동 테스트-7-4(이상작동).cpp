

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
 * robot_arm_driving_7th_generation.cpp
 *
 * Created: 2020-03-02 ���� 3:19:24
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

//<��ó��>00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
#define F_CPU 16000000UL //������� ũ������ ���� ����� �����ݴϴ�. (16MHz)
//#include <avr/io.h>
//#include <avr/interrupt.h> //���ͷ�Ʈ ���� ��������� �����մϴ�.
//#include <util/delay.h>
#include <math.h>
//<����>========================================


//<����ü ����>111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111


struct position_management
{
	//ractangular_coordinates
	double macroscopic_current_position_x = 0;
	double macroscopic_current_position_y = 0;
	double macroscopic_current_position_z = 0;

	double macroscopic_destination_position_x = 0;
	double macroscopic_destination_position_y = 0;
	double macroscopic_destination_position_z = 0;

	//��ġ�� ������ ���� ����� �ؾ� �ϴ� �κ����� ����� ���� ���� ����. 2020.02.22
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
	//�ٲ�� ����� �������� Ư��
	int motor_type = 'v';//'s' step,'b' bldc
	int step_per_revolution_presison;//�ӽð� 100.
	
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
	char error;
	int cumulated_steps;
	
	//hall_sensor_only, Ȧ���� ����
	int current_hall_sensor_value;// = -1
	int just_before_hall_sensor_value;// = -1
	
	//���� ���� �� ���� ����.
	int microscopic_displacement_value;
	char direction_value;

	double progress_situation;// = 0//�����Ȳ
	char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.

	//<��>==================================================
	
	//��������� �̿�Ǵ� ����.
	double current_degree = 0;
	double destination_degree = 0;
	long displacement_step;
	long current_step;
	long destination_step;
};

struct motor_list
{
	struct motor_mathod m1_stepper;//�� 1���� ����
	struct motor_mathod m2_stepper;//�� 2���� ����_step_motor_mode
	//struct motor_mathod m2_bldc;//�� 2���� ����
	struct motor_mathod m3_stepper;//�� 3���� ����
	struct motor_mathod m4_stepper;//�� 4���� ����
	struct motor_mathod m5_servo_gripper;
	void* ending_NULL_pointer;// = NULL
	
	double length_1 = 26;
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

//<��� ����>222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222

//<����>��� ����================================
int encoder_management(struct motor_mathod* motor_name);
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress);
//char port_input_assistant(char port_name, char object_pin);
char bit_analysis(int bit_old, int bit_new);//default parameter : bit_old = 0b000, bit_new = 0b000
//<��>��� ����==================================

//<��ź�>333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333


//<�⺻ �м� �Լ�>44444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444
//<����>========================================
//<����>========================================
int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress)//���� ���縦 ���ؼ� �ּҸ� ����//it takes adress for deep copy
{
	//�ٲ��� ������쿡�� �Լ��� ����Ǿ� old�� new�� ���� ���� ���� �Ǿ� ������ ��Ȳ�� �����ϴ� ��� �߰�
	if(*bit_new_s_adress == -1)//�ʱ� �� �˻縦 -1�� �ϴ� ����������?
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
//<��>==========================================
//<����>========================================
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
//<��>==========================================

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



//<�⺻ ���� �Լ�>55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555
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

void delay_costom_us(int time_us = 0)
{
	int n = 0;
	while(n < time_us)
	{
		_delay_us(1);
		n=n+1;
	}
}



//<��>==========================================

//���� ����� ���� �Լ�
//<����>========================================
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
//<��>==========================================
//<����>========================================
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
//<��>==========================================



//<��� �м� ���� �Լ�>66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666
//<����>========================================
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
//<��>==========================================

//<���� �� �׽�Ʈ�� �Լ� ����>77777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
//���� ��ġ ��� �Լ�
//<����>========================================
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
//<��>========================================
//���� ��ȣ ��� �Լ�

//<��ΰ�� ���� �Լ�>88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
//<����>========================================
//����κа� �� �κ��� �����Ͽ� ����� ������.
//���� �������� �ⱸ�� ���´� 3���� �� ���̿� ���� ��Ÿ��. �������� ��ġ�� ���ؼ��� ��������� �����ο� ��
//���̿� ���� ���� ������ �������� ���¸� ���� �� �ֳ�??
//<��>========================================
//<����>========================================
//�ʿ��� �Լ� ���� ����
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
//<��>========================================
//<����>========================================
void combine_macroscopic_to_micro_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{

	(*robot_arm_address).position_value.microscopic_destination_position_x =
	(*robot_arm_address).position_value.macroscopic_current_position_x + ((*robot_arm_address).position_value.macroscopic_destination_position_x - (*robot_arm_address).position_value.macroscopic_current_position_x) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_y =
	(*robot_arm_address).position_value.macroscopic_current_position_y + ((*robot_arm_address).position_value.macroscopic_destination_position_y - (*robot_arm_address).position_value.macroscopic_current_position_y) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_z =
	(*robot_arm_address).position_value.macroscopic_current_position_z + ((*robot_arm_address).position_value.macroscopic_destination_position_z - (*robot_arm_address).position_value.macroscopic_current_position_z) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	//����� �κ� ������ ����, function part�� ���� ��� func ������ position ����ü�� �̿��� ��, motor_mathod�� �̿����� ������ �� ����.
	
	
	(*function_name_address).microscopic_destination_position_seta = (*function_name_address).macroscopic_current_position_seta + ((*function_name_address).macroscopic_destination_position_seta - (*function_name_address).macroscopic_current_position_seta) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	(*function_name_address).microscopic_destination_position_grip = (*function_name_address).macroscopic_current_position_grip + ((*function_name_address).macroscopic_destination_position_grip - (*function_name_address).macroscopic_current_position_grip) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);

}

//<��>========================================
//<����>========================================
int function_part_position_transformation(double seta, double length, double x, double y, double z, double* address_of_x, double* address_of_y, double* address_of_z)
{
	//�������� ��츸 �����ϵ��� ���� �Ѵٸ�.

	if((seta-M_PI_2)*(seta-M_PI_2)>M_PI_2*M_PI_2)//������ ���ٰ����ԵǸ� func_seta�� ������  0,�� ���� M_PI�� ��쿡 ���� �� �߻���
	{
		return 0;//���⼭ ���� 0�� �߻��Ͽ� �Լ��� ���ڱ� �����. Ư���� ó��°���� �ɸ��� ��� ���??
	}
	// ���� �Լ��� ���  DEGREE_OF_SETA �� atan2(y, x)�� ���� �����Ǵ� ��� �̹Ƿ� SETA�� ���� �߿� ���� ����.
	//�� �ƴϸ��� role ������ �ִ� ��� �̹Ƿ� atan2(y, x)�� ���� ���� ���� ��.
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
//<��>========================================
//<����>========================================
int robot_4aix_arm_degree_clac(struct motor_list* robot_arm_address, double length_1, double length_2, double length_3, 
double x_destination, double y_destination , double z_destination, double function_target_degree, double* m1_degree, double* m2_degree, double* m3_degree, double* m4_degree, double* cumulated_angle_position)
{

	double r_0 = sqrt(x_destination*x_destination + y_destination*y_destination + z_destination*z_destination);
	
	
	
	double yaw_0 = atan2(y_destination, x_destination);
	
	
	
	double pitch_0 = atan2(z_destination, sqrt(x_destination*x_destination + y_destination*y_destination));//sholder
	double phi_0 = M_PI_2-pitch_0;
	double side_1 = sqrt(length_1*length_1 + r_0*r_0 - 2*length_1*r_0*cos(phi_0));
	
	//��� �� ������ 90�ΰ�� �ٻ������� +������ �߻��Ͽ� asin�� ������ ����� ������ ����. ���� 1���ٰ��� �� ū ��� ������ �ʿ���. �����̵� sin�� ġ���� -1~1������� Ư���� Ȱ���Ͽ�  ��������. 
	//�Ǵ� ��2 �ڻ��� ��Ģ�� �̿��ϴ°�쿡 ������ �߻����� �ʴ��� Ȯ���غ��� ������ ������ ������ �� �� ����. 
	
	/*
	x_destination, y_destination, z_destination�� 0�̵Ǿ�,side_1�� 0�� �Ǵ� ��쿡�� 
	[r_0 / side_1 * sin(phi_0)]�� ���� �������� 0���� ������ ������ �߻��� �� �ִ�.
	�׷��� �̷� ���� ��ǥ��ǥ�� 0���� �����ϰų�, ���� ��ǥ�� 0���� �����ϴ� ����̹Ƿ� ��� �����ϴ�. 
	�̷� ���� ������ǥ�� �����̰� ��ǥ ��ǥ�� ������ �Ǵ� ���͸� ���ϰ�, 
	Z���� ������ �����϶� �عٴڿ� �ش��ϴ� XY���� �ռ����� ���ͻ����� ���� ���� ������ ���� ���踦 ���� ����ϸ� �ȴ�.
	������ ������ (0, 0, length_1)���� �����ϴ� ���� ��� �� ���� ������ (0, 0, length_1)�� ���� ����� ��  
	���� ���ͻ����� ������ XY��鿡 ���Ͽ� �����Ǿ��ִ� ���¸� �������� ��꿡�� ���ǰ� �ʿ��ϴ�. 
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
			//������ ��ǥ�� (0, 0, length_1)�� ��� 
			/*
			���� ����� ������, ������ �Ǵ� ������ x��� ���������̹Ƿ� 
			��� �Ǵ� ������ x��� seta��ŭ�� ������ �̷�� �ش��� pitch_1�϶�
			pitch_1 = 90 + seta;
			�׸��� ��ǥ�͸� �˰� �������� seta = atan2(y, x)�� �����ϸ� 
			*/
			pitch_1 = M_PI_2 + atan2(delta_z, delta_r_proj_xy);
		}
		else
		{
			/*
			������ x��� ������ �̷�� ������ ��ġ ���Ͱ� ������  (0, 0, length_1)�� ���� ���� ����Ī�� ���迡 ��������
			�� ��ǥ�� (-)��ȣ�� �ٿ� ����ϸ� ���� pitch_1�� ����� �� ���� ���̴�. 
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
//<��>========================================
//<����>========================================
/*
void degree_to_step(struct motor_list* robot_arm_address, int number_of_motors)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	double rad_per_step = 2*M_PI/(double)(*motor_Ptr).step_per_revolution_presison;
	double gear_ratio = (*motor_Ptr)
	
	int i = 0;
	while(i<number_of_motors)
	{
		(*motor_Ptr).destination_step = (long)  (((*motor_Ptr).destination_degree / (2*M_PI))* (double)(*motor_Ptr).step_per_revolution_presison);
		motor_Ptr++;
		i = i + 1;
	}
}
*/
void degree_to_step(struct motor_list* robot_arm_address, int number_of_motors)
{
	//struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	//double rad_per_step = 2*M_PI/(double)(*motor_Ptr).step_per_revolution_presison;
	//double gear_ratio = (*motor_Ptr)
	//(*motor_Ptr).destination_step = (long)  (((*motor_Ptr).destination_degree / (2*M_PI))* (double)(*motor_Ptr).step_per_revolution_presison);
	
	robot_arm_1.m1_stepper.destination_step = (long)  ((robot_arm_1.m1_stepper.destination_degree / (2*M_PI))* 21525);
	robot_arm_1.m2_stepper.destination_step = (long)  ((robot_arm_1.m2_stepper.destination_degree / (2*M_PI))* 38745);
	robot_arm_1.m3_stepper.destination_step = (long)  ((robot_arm_1.m3_stepper.destination_degree / (2*M_PI))* -12915);
	robot_arm_1.m4_stepper.destination_step = (long)  ((robot_arm_1.m4_stepper.destination_degree / (2*M_PI))* 1435);
	robot_arm_1.m5_servo_gripper.destination_step = (long)  ((robot_arm_1.m5_servo_gripper.destination_degree / (2*M_PI))* (double)robot_arm_1.m5_servo_gripper.step_per_revolution_presison);
	

}
//<��>========================================
//<����>========================================
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
	(*function_name_address).microscopic_destination_position_seta, // ������� �� �� ���� Ȯ�� �� ����.
	//m2_bldc_mode
	//&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	//m2_step_motor_mode
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_stepper.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	);
	
	(*robot_arm_address).m5_servo_gripper.destination_degree = (*function_name_address).microscopic_destination_position_grip;
	
	
	degree_to_step(&robot_arm_1, 5);
	
}
//<��>========================================
//<����>========================================
void rectangular_coordinates_microscopic_to_driver_ordering(struct motor_list* robot_arm_address)
{//���� �帮�̹��� ���� ��ǥ���� ���·� �����̴��� sin, cos tan �� �ﰢ �Լ��� ���¸� ���ϰ� �������� ���ǹ�
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
//<��>========================================


//<��� �����, ���� ���� �����Լ�>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//<����>========================================
void output_safety_function(char port_name ,char object_pin)//���Ϳ� ������� ���Ǵ� �ɷ������͸� �����ϴ� ��� �߰� �ʿ�.!!!!!!!!!!!!!!!!!!!!!!!!!
{
	port_output_assistant(port_name, object_pin, 0b0000000);
	_delay_us(5);
}
//<��>==========================================
//<����>========================================
int hall_sensor_reding(char port_name, char pin_data = 0b00000111)
{
	return  port_input_assistant(port_name, pin_data);
}
//<��>==========================================
//<����>========================================
void input_value_to_brige_open_loop(char port_name, char object_pin ,char binary_char = 0b00000000, int time_ms = 0)//a part of auto output system.
{
	port_output_assistant(port_name, object_pin, binary_char);
	delay_costom_ms(time_ms);//it should be changed, it can control several of motors in same time. doesn't motors are stopped by that delay function.
	output_safety_function(port_name, object_pin);
}
//<��>==========================================
//<����>========================================
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
			holding_value = 0;//enable �� 1�� ��쿡�� ���� ����� �ǹ���.
			break;
		case 2: // backward
			dir_forward_back = 0;
			step_value = 1;
			holding_value = 0;
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
		delay_costom_us(900);
		out_put_value = 0;
		out_put_value |= (dir_forward_back<<2)|(0<<1)|(holding_value);
		port_output_assistant(motor_port, motor_pin, out_put_value);
		delay_costom_us(900);
	}
	//output_safety_function(motor_port, motor_pin);
	//delay_costom_us(400);
	virtual_position_management(current_step_address, *direction_value);
	//current_position_value ������Ű��.
}
//<��>==========================================
//<����>========================================
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
//<��>==========================================
//<����>======================================
void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value_address, int* just_before_hall_sensor_value_address, char* direction_value_address, long* current_step_address)//, char hall_sensor_port, int hall_sensor_pin �� Ȧ���� ���б�� �ǽð� ���ð� �߿��ϱ⿡ Ÿ�̸� ���ͷ�Ʈ�� ��� ����Ѵ�.
{
	//�� �Լ����� ���ܸ���/bldc, ���� Ȧ�� ����, ��Ʈ�� �ɼ���, ���� , ��� ���� �Ǵ��Ͽ� ���� �Լ��鿡�� ���������.
	
	switch(motor_type)
	{
		case 'b':
		case 'B':
		//bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value_address, just_before_hall_sensor_value_address, direction_value_address);//�Ķ���͸� ���� ������ ���� �Լ��� �������� ���� ���� ���� �ٸ� �� �ֱ⶧���� Ÿ�̸� ���ͷ�Ʈ�� ��� �ʱ�ȭ �ϴ� ���� �޸� �ּҿ� �����͸� �̿��Ͽ� �����Ѵ�.
		break;
		case 's':
		case 'S':
		step_motor_driving_open_loop(motor_port, motor_pin, direction_value_address, current_step_address);//������ Ÿ�ӵ� �־�ָ� ������ ���� ��� ������. �� ����ü�� �̸� �����صξ� ������ �Ӽ��� ������ ������ ����ϱ�.
		break;
		case 'v':
		case 'V':
		virtual_position_management(current_step_address, *direction_value_address);
		break;
	}
}

//<��>========================================
//<����>========================================
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
//<��>========================================

//<���ۿϷ� ���� �Լ�>BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
//<����>========================================
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

//<��>========================================
//<����>========================================
int rectangular_coordinate_microscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	//�̽��� �ܰ迡�� ��� �ذ��ұ�.....? .���� ��������  ������ġ�� �Ϸ� ���¸� Ȯ�� �Ͽ��� �� �� ��.
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	while(n < 5)
	{

		(*motor_Ptr).displacement_step = ((*motor_Ptr).destination_step - (*motor_Ptr).current_step);
		if((*motor_Ptr).displacement_step != 0)
		{
			(*motor_Ptr).microscopic_complition = 'n';
			//�̺κ��� micro to diriver �κ����� �Ű� �� �� ��. 
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
//<��>========================================

//<��ġ �ʱ�ȭ>CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//<����>========================================
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
	purpose_seta, // ������� �� �� ���� Ȯ�� �� ����.
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
//<��>========================================


//<�� ���� �Լ�>DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
void whole_driving()
{
	while(rectangular_coordinate_macroscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
	{
		combine_macroscopic_to_micro_ordering(&robot_arm_1, &func_gripper);
		ordering_trans_form(&robot_arm_1, &func_gripper);
		
		while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
			printf("PORTD : %x\n", PORTD); 
		}
		robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
		
		
		//_delay_us(300);
		step_motor_power_saving(&robot_arm_1, 5);//2020.02.29 15:45 �ű� �߰�.
		//microscopic_current_position_clac(&robot_arm_1, 4);
		//PORTD = 0b00000000;
		//robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
	}
}


//<Ÿ�̸� �� ��ġ ���ͷ�Ʈ>EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE



//<�� ��ɹ�>FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
int main(void)
{
	
	
	//UART1_init();
	//<����>���� �ʱⰪ ����=========================================
	DDRE = 0b11111111;
	DDRC = 0b11111111;
	DDRD = 0b11111111;
	DDRB = 0b11111111;
	//PORTB = 0b11111111;
	//<����>m1 �ʱⰪ ����=========================================
	robot_arm_1.m1_stepper.motor_type = 's';
	robot_arm_1.m1_stepper.motor_driving_mode = 'n';
	robot_arm_1.m1_stepper.step_per_revolution_presison = 21525;//1435*3*5
	robot_arm_1.m1_stepper.proper_direction = 1;
	
	robot_arm_1.m1_stepper.motor_port = 'c';
	robot_arm_1.m1_stepper.motor_pin = 0b00000111;
	//robot_arm_1.m1_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m1_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m1_stepper.just_before_hall_sensor_value = -1;

	//<��>m1 �ʱⰪ ����=========================================
	
	//<����>m2 �ʱⰪ ����=========================================
	robot_arm_1.m2_stepper.motor_type = 's';
	robot_arm_1.m2_stepper.motor_driving_mode = 'n';
	robot_arm_1.m2_stepper.step_per_revolution_presison = 1435;//1435*3*3*5
	robot_arm_1.m2_stepper.proper_direction = 1;
	
	robot_arm_1.m2_stepper.motor_port = 'd';
	robot_arm_1.m2_stepper.motor_pin = 0b00111000;
	//robot_arm_1.m2_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m2_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m2_stepper.just_before_hall_sensor_value = -1;

	/*
	//<����>m2_bldc_mode=========================================
	robot_arm_1.m2_bldc.motor_type = 'b';
	robot_arm_1.m2_bldc.motor_driving_mode = 'n';
	robot_arm_1.m2_bldc.step_per_revolution_presison = 100;;
	robot_arm_1.m2_bldc.proper_direction = 1;
	
	robot_arm_1.m2_bldc.motor_port = 'b';
	robot_arm_1.m2_bldc.motor_pin = 0b00111111;
	robot_arm_1.m2_bldc.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m2_bldc.current_hall_sensor_value = -1;
	robot_arm_1.m2_bldc.just_before_hall_sensor_value = -1;
	//<��>m2_bldc_mode=========================================
	*/
	//<��>m2 �ʱⰪ ����=========================================
	//<����>m3 �ʱⰪ ����=========================================
	robot_arm_1.m3_stepper.motor_type = 's';
	robot_arm_1.m3_stepper.motor_driving_mode = 'n';
	robot_arm_1.m3_stepper.step_per_revolution_presison = 12915;//1435*3*3
	robot_arm_1.m3_stepper.proper_direction = 1;
	
	robot_arm_1.m3_stepper.motor_port = 'b';
	robot_arm_1.m3_stepper.motor_pin = 0b00111000;
	//robot_arm_1.m3_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m3_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m3_stepper.just_before_hall_sensor_value = -1;

	//<��>m3 �ʱⰪ ����=========================================
	//<����>m4 �ʱⰪ ����=========================================
	robot_arm_1.m4_stepper.motor_type = 's';
	robot_arm_1.m4_stepper.motor_driving_mode = 'n';
	robot_arm_1.m4_stepper.step_per_revolution_presison = 1435*1;
	robot_arm_1.m4_stepper.proper_direction = 1;
	
	robot_arm_1.m4_stepper.motor_port = 'e';
	robot_arm_1.m4_stepper.motor_pin = 0b11100000;
	//robot_arm_1.m4_stepper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m4_stepper.current_hall_sensor_value = -1;
	robot_arm_1.m4_stepper.just_before_hall_sensor_value = -1;

	//<��>m4 �ʱⰪ ����=========================================
	//<����>m5 �ʱⰪ ����=========================================
	robot_arm_1.m5_servo_gripper.motor_driving_mode = 'n';
	robot_arm_1.m5_servo_gripper.step_per_revolution_presison = 1;
	robot_arm_1.m5_servo_gripper.proper_direction = 1;
	
	robot_arm_1.m5_servo_gripper.motor_port = 'b';
	robot_arm_1.m5_servo_gripper.motor_pin = 0b01000000;
	//robot_arm_1.m5_servo_gripper.hall_sensor_pin = 0b00000111;
	
	robot_arm_1.m5_servo_gripper.current_hall_sensor_value = -1;
	robot_arm_1.m5_servo_gripper.just_before_hall_sensor_value = -1;

	//<��>m5 �ʱⰪ ����=========================================
	step_motor_power_saving(&robot_arm_1, 5);
	robot_arm_1.ractangular_macro_compliment_value = 'y';
	//<��>���� �ʱⰪ ����===========================================
	/*
	//<����>Ÿ�̸� ���ͷ�Ʈ ���� ����=================================
	TCCR0 = (1<<WGM00) | (1<<CS0);
	
	TCNT0 = 0;
	
	OCR0 = 10;
	
	TIMSK |= (1<<OCIE0);
	
	sei();
	//<��>Ÿ�̸� ���ͷ�Ʈ ���� ����===================================
	*/
	
	
	//current_position_caliberation(&robot_arm_1.m2_bldc);//�뵵�� ����? = ����Ķ���� ���̼� ���� ȣȯ. ���� ������� ����.
	
	/*�׽�Ʈ �� ��Ȱ��ȭ.
	//<����>���� �ʱ�ȭ.=============================================
	motor_combo_calibration(&robot_arm_1, &func_gripper, 20, 0, 37, 0, 255, 5);//last argument is number_of_motors
	//<��>���� �ʱ�ȭ.=============================================
	//<����>��ġ ��ɹ� ����=======================================
	combine_ordering(&robot_arm_1, &func_gripper, 20, 0, 37, 2*M_PI, 255, 100);
	whole_driving();
	//<��>��ġ ��ɹ� ����=========================================
	*/
	
	
	///*
	//<����>���� ���� ��ġ���� �׽�Ʈ==============================================================
	_delay_ms(1000);
	//PORTE = 0b00000001;
	//_delay_ms(1000);
	
	motor_combo_calibration(&robot_arm_1, &func_gripper, 36, 0, 10,0, 255, 5);//last argument is number_of_motors
	//=============================
	///*
	
	combine_ordering(&robot_arm_1, &func_gripper, 36, 0, 15, 0, 255, 1000);
	
	whole_driving();
	
	_delay_ms(1000);
	
	combine_ordering(&robot_arm_1, &func_gripper, 36, 0, 10, 0, 255, 1000);

	whole_driving();
	//==============================
	//combine_ordering(&robot_arm_1, &func_gripper, 28.5, 0, 4.5, M_PI, 255, 1000);

	//whole_driving();
	
	//combine_ordering(&robot_arm_1, &func_gripper, 28.5, 0, 4.5, M_PI_2, 255, 1000);

	//whole_driving();
	
	//*/
	//===============================
	/*
	combine_ordering(&robot_arm_1, &func_gripper, 36, 5, 10, M_PI_2, 255, 1000);
	
	whole_driving();
	
	combine_ordering(&robot_arm_1, &func_gripper, 36, 0, 10, M_PI_2, 255, 1000);

	whole_driving();
	//===================================
	combine_ordering(&robot_arm_1, &func_gripper, 36, -5, 10, M_PI_2, 255, 1000);

	whole_driving();
	
	combine_ordering(&robot_arm_1, &func_gripper, 36, 0, 10, M_PI_2, 255, 1000);
	
	whole_driving();
	//*/
	//=================================
	
	step_motor_power_saving(&robot_arm_1, 5);
	//PORTE = 0b00000000;
	//<��>���� ���� ��ġ���� �׽�Ʈ==============================================================
	//*/
	

	/*
	//<����>�������� ����׽�Ʈ==================================================(����)
	_delay_ms(1000);
	//PORTD = 0b00000001;
	//int k = 0;
	//for(k = 0; k < 5;k = k+1)
	//{
		
		
		robot_arm_1.m2_stepper.current_degree = 0;
		robot_arm_1.m2_stepper.destination_degree = M_PI/4;
		
		
		degree_to_step(&robot_arm_1, 4);
		while (rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
		}
		step_motor_power_saving(&robot_arm_1, 5);
		
		
		
		
		robot_arm_1.m2_stepper.destination_degree = 0;
		
		
		
		
		degree_to_step(&robot_arm_1, 4);
		while (rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			rectangular_coordinates_microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
		}
		step_motor_power_saving(&robot_arm_1, 5);

		
		
		
		
		
	//}

	//PORTD = 0b00000000;
	//<��>�������� ����׽�Ʈ==================================================
	*/
	
	
	/*
	//<��>���� ���� �׽�Ʈ_1_========================================
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
	//<��>========================================

	*/
/*
	while(1)
	{
		
	}
*/
	return 0;
}
//<��>========================================



