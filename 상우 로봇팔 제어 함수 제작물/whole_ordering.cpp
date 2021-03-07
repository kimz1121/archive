#include<stdio.h>
#include<math.h>


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
		char error;
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
	
	char robot_dirvig_compliment_value;
	
	double macroscopic_current_position_x;
	double macroscopic_current_position_y;
	double macroscopic_current_position_z;
	
	double macroscopic_destination_position_x;
	double macroscopic_destination_position_y;
	double macroscopic_destination_position_z;
	
	double microscopic_current_position_x;
	double microscopic_current_position_y;
	double microscopic_current_position_z;
	
	double transformed_current_position_x;
	double transformed_current_position_y;
	double transformed_current_position_z;
	
	double microscopic_destination_position_x;
	double microscopic_destination_position_y;
	double microscopic_destination_position_z;
	
	double transformed_purpose_position_x;
	double transformed_purpose_position_y;
	double transformed_purpose_position_z;
	
	
	
	int munber_of_motors;
	
}robot_arm_1;


int rectangular_coordinates_first_class_order(double purpose_position_x, double purpose_position_y, double seta, double grip, double purpose_position_z, double* destination_position_x, double* destination_position_y, double* destination_position_z)
{
	if(robot_dirvig_compliment_value == 1)
	{
		*destination_position_x = purpose_position_x;
		*destination_position_y = purpose_position_y;
		*destination_position_z = purpose_position_z;
	}
}

int rectangular_coordinates_macroscopic_to_micro_ordering(struct motor_list* robot_name, double time_out)
{
	(*robot_name).microscopic_destination_position_x = (*robot_name).macroscopic_current_position_x + ((*robot_name).macroscopic_destination_position_x - (*robot_name).macroscopic_current_position_x) * ((*robot_name).stream_of_time / time_out);
	(*robot_name).microscopic_destination_position_y = (*robot_name).macroscopic_current_position_y + ((*robot_name).macroscopic_destination_position_y - (*robot_name).macroscopic_current_position_y) * ((*robot_name).stream_of_time / time_out);
	(*robot_name).microscopic_destination_position_z = (*robot_name).macroscopic_current_position_z + ((*robot_name).macroscopic_destination_position_z - (*robot_name).macroscopic_current_position_z) * ((*robot_name).stream_of_time / time_out);	
}

int function_ordering(seta, grip)

