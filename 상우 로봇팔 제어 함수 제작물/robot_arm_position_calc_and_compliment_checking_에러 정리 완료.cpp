#include<stdio.h>
#include<math.h>


 

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
	int step_per_revolution_presison = 100;//�ӽð� 100. 
	
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
	int direction_value;


	double progress_situation;// = 0//�����Ȳ
	char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	
	
	//<s>������� �ƴ� ���� ��� �ʿ��� ����.===========
	long macroscopic_destination_position;// = 0//�Ž��� ��� ���� �Լ����� Ȱ��
	long macroscopic_current_position;// = 0
	
	long microscopic_destination_position;// = 0//�̽��� ��� ���� �Լ����� ���
	long microscopic_current_position;// = 0
	//<e>==================================================
	
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
	struct motor_mathod m2_bldc;//�� 2���� ����
	struct motor_mathod m3_stepper;//�� 3���� ����
	struct motor_mathod m4_stepper;//�� 4���� ����
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

//header list.
int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address);


int direction_value_clac_arg_2(int displacement, int driving_mode)
{
	printf("displacement :  %d\n", displacement);
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





int combine_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, double purpose_time_out)
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

int function_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double seta, double grip)
{
	
}

int position_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double x, double y, double z)
{
	
}



int combine_macroscopic_to_micro_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address)
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




int degree_to_step(struct motor_list* robot_arm_address, int number_of_motors)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	double rad_per_step = 2*M_PI/(double)(*motor_Ptr).step_per_revolution_presison;
	
	int i =0;
	while(i<number_of_motors)
	{
		(*motor_Ptr).destination_step = (long)(((*motor_Ptr).destination_degree * (double)(*motor_Ptr).step_per_revolution_presison) / (2*M_PI));
		motor_Ptr++;
		i = i + 1;
	}
}

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
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	
	);
	
	(*robot_arm_address).m5_servo_gripper.destination_degree = (*function_name_address).microscopic_destination_position_grip;
	
	
	degree_to_step(&robot_arm_1, 5);
	
}



void microscopic_to_driver_ordering(struct motor_list* robot_arm_address)
{
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






int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{

	printf("%c\n", (*robot_arm_address).ractangular_macro_compliment_value);
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

int rectangular_coordinate_microscopic_compliment_checking(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	//�̽��� �ܰ迡�� ��� �ذ��ұ�.....? .���� ��������  ������ġ�� �Ϸ� ���¸� Ȯ�� �Ͽ��� �� �� ��.
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	//printf("\n\n");
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
	//printf("\n\n");
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



void virtual_position_management(long* current_position, int direction_value)//use it with delay. 
{
	switch(direction_value)
	{
		case 1:
			printf("forward\n");
			printf("current position : %d\n", *current_position);
			*current_position = *current_position + 1;
			break;
		case 2:
			printf("backward\n");
			printf("current position : %d\n", *current_position);
			*current_position = *current_position - 1;
			break;
	}
}

void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* direction_value, long* current_step_aderess)//, char hall_sensor_port, int hall_sensor_pin �� Ȧ���� ���б�� �ǽð� ���ð� �߿��ϱ⿡ Ÿ�̸� ���ͷ�Ʈ�� ��� ����Ѵ�. 
{
	//�� �Լ����� ���ܸ���/bldc, ���� Ȧ�� ����, ��Ʈ�� �ɼ���, ���� , ��� ���� �Ǵ��Ͽ� ���� �Լ��鿡�� ���������.
	
	switch(motor_type)
	{
		case 'b':
		case 'B':
			//bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value, just_before_hall_sensor_value, direction_value);//�Ķ���͸� ���� ������ ���� �Լ��� �������� ���� ���� ���� �ٸ� �� �ֱ⶧���� Ÿ�̸� ���ͷ�Ʈ�� ��� �ʱ�ȭ �ϴ� ���� �޸� �ּҿ� �����͸� �̿��Ͽ� �����Ѵ�.
			break;
		case 's':
		case 'S':
			//step_motor_driving_open_loop(motor_port, motor_pin, direction_value, current_step_aderess);//������ Ÿ�ӵ� �־�ָ� ������ ���� ��� ������. �� ����ü�� �̸� �����صξ� ������ �Ӽ��� ������ ������ ����ϱ�. 
			break;
		case 'v':
		case 'V':
			virtual_position_management(current_step_aderess, *direction_value);
			break;
	}
}


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

//�����ʿ� ������� ������ 
void microscopic_current_position_clac(struct motor_list* robot_arm_address, int number_of_arm_motor)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	double* length_Ptr = &((*robot_arm_address).length_1);
	
	int n = 0;
	
	double x = 0;
	double y = 0;
	double z = 0;
	
	double degree_roll = 0;
	double degree_yaw = (*robot_arm_address).m1_stepper.current_degree;
	double degree_pitch = 0;
	
	z = z + (*robot_arm_address).length_1;
	length_Ptr++;
	motor_Ptr++;
	while(n < number_of_arm_motor-1)//3�� �ݺ� ������ ���� ����.
	{

		double r_proj_xy = (*length_Ptr)*sin(degree_pitch);
		x = x + r_proj_xy*cos(degree_yaw);
		y = y + r_proj_xy*sin(degree_yaw);
		z = z + (*length_Ptr)*cos(degree_pitch);
		
		n = n + 1;
		length_Ptr++;
		motor_Ptr++;
	}
	
	
	/*�ٽɿ��� �߻��κ�. ��������� ���� 
	(*robot_arm_address).position_value.microscopic_current_position_x = x;
	(*robot_arm_address).position_value.microscopic_current_position_y = y;
	(*robot_arm_address).position_value.microscopic_current_position_z = z;
	*/
}






//�����ʿ� 
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
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	
	);
	
	
	
	printf("check 6 : %f, %f, %f, %f, %f\n", robot_arm_1.m1_stepper.destination_degree, robot_arm_1.m2_bldc.destination_degree, robot_arm_1.m3_stepper.destination_degree, robot_arm_1.m4_stepper.destination_degree, robot_arm_1.m5_servo_gripper.destination_degree);
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
	
	
	printf("\n���� step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
	
	printf("\n\n%f, %f, %f,/\n %f, %f, %f\n", 
			purpose_position_x, 
			purpose_position_y, 
			purpose_position_z, 
			robot_arm_1.position_value.transformed_current_position_x, 
			robot_arm_1.position_value.transformed_current_position_y, 
			robot_arm_1.position_value.transformed_current_position_z
		 );
}


void whole_driving()
{
	while(rectangular_coordinate_macroscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
	{
		printf("c\n");
		
		combine_macroscopic_to_micro_ordering(&robot_arm_1, &func_gripper);

		ordering_trans_form(&robot_arm_1, &func_gripper); 
		
		

		
		//printf("\ncheck1 %d\n", rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1));
		while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{

			
			//calc_derection_value(&robot_arm_1); 
			microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
			//printf("\n%d, %d\n\n", robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
			//printf("\n%f, %f\n\n", robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
//			printf("\n���� step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
			//printf("�̽ø�ǥ  step %f, %f, %f, %f, %f\n", robot_arm_1.m1_stepper.destination_step, robot_arm_1.m2_bldc.destination_step, robot_arm_1.m3_stepper.destination_step, robot_arm_1.m4_stepper.destination_step, robot_arm_1.m5_servo_gripper.destination_step);
			
			
			//microscopic_current_position_clac(&robot_arm_1, 4);
			
			
		}
		
		printf("degree check  : %f, %f, %f, %f, %f\n", robot_arm_1.m1_stepper.displacement_step, robot_arm_1.m2_bldc.destination_degree/M_PI*180, robot_arm_1.m3_stepper.destination_degree/M_PI*180, robot_arm_1.m4_stepper.destination_degree/M_PI*180, robot_arm_1.m5_servo_gripper.destination_degree/M_PI*180);

			printf("\nposition check  : \n%f, %f, %f,/\n %f, %f, %f\n", 
			robot_arm_1.position_value.microscopic_destination_position_x, 
			robot_arm_1.position_value.microscopic_destination_position_y, 
			robot_arm_1.position_value.microscopic_destination_position_z, 
			robot_arm_1.position_value.transformed_destination_position_x, 
			robot_arm_1.position_value.transformed_destination_position_y, 
			robot_arm_1.position_value.transformed_destination_position_z
			);
		
			printf("displacement check  : \n%d, \n%d, \n%d, \n%d, \n%d\n", robot_arm_1.m1_stepper.displacement_step, robot_arm_1.m2_bldc.displacement_step, robot_arm_1.m3_stepper.displacement_step, robot_arm_1.m4_stepper.displacement_step, robot_arm_1.m5_servo_gripper.displacement_step);
			printf("current_step check  : \n%d, \n%d, \n%d, \n%d, \n%d\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
			
		//microscopic_current_position_clac(&robot_arm_1, 4); 
		
		robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
	}
	
}

int main()
{
	robot_arm_1.m1_stepper.step_per_revolution_presison = 100;
	robot_arm_1.m2_bldc.step_per_revolution_presison = 100;
	robot_arm_1.m3_stepper.step_per_revolution_presison =100;
	robot_arm_1.m4_stepper.step_per_revolution_presison = 100;
	robot_arm_1.m5_servo_gripper.step_per_revolution_presison = 1;
	
	robot_arm_1.ractangular_macro_compliment_value = 'y';
	
	motor_combo_calibration(&robot_arm_1, &func_gripper, 20, 0, 37, 0, 255, 5);//last argument is number_of_motors
	printf("\n���� step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
	
 	
//	current_position_caliberation(&robot_arm_1);
//	reset_trans_form(&robot_arm_1, &func_gripper); 

	printf("a");
	while(1)
	{
		combine_ordering(&robot_arm_1, &func_gripper, 20, 0, 37, 2*M_PI, 255, 100);
		whole_driving();
		/*
		combine_ordering(&robot_arm_1, &func_gripper, 30, 30, 27, 0, 255, 100);
		whole_driving();
		printf("==============================================================\n==============================================================");
		combine_ordering(&robot_arm_1, &func_gripper, 15, 0, 17, M_PI_2, 255, 100);
		whole_driving();
		printf("==============================================================\n==============================================================");
		combine_ordering(&robot_arm_1, &func_gripper, 30, 30, 27, 0, 255, 100);
		whole_driving();		
		printf("==============================================================\n==============================================================");
		*/
		break;
	}
} 

//���� ���������� �ʱ�ȭ�� ��� �ѹ��̶� ���� ������ ������ �߻���. 
//���� ������ ���� ������ �Ŵ��ϰ� �߻��ϴ� ��찡 ���� � ����ϱ�....? 
//�ϴ� purpose seta�� ��ǥ ������  0�� �ƴҶ� ������ ���� ������ �����. 











