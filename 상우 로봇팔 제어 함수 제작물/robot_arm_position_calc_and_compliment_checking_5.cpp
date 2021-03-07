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
	int step_per_revolution_presison = 100;//임시값 100. 
	
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
	int direction_value;


	double progress_situation;// = 0//진행상황
	char macroscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	char microscopic_complition;// = 0//standard 0 , present progressive -1, 1 finished.
	
	
	//<s>직교제어가 아닌 각도 제어에 필요한 변수.===========
	long macroscopic_destination_position;// = 0//거시적 경로 설정 함수에서 활용
	long macroscopic_current_position;// = 0
	
	long microscopic_destination_position;// = 0//미시적 경로 설정 함수에서 사용
	long microscopic_current_position;// = 0
	//<e>==================================================
	
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
	struct motor_mathod m2_bldc;//제 2관절 모터
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
	
	char ractangular_macro_compliment_value;
	char ractangular_micro_compliment_value;
	
	struct position_management position_value;

	double cumulated_angle_position;
	
	int munber_of_motors;

	

}robot_arm_1;



//int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address);



int direction_value_clac_arg_2(int displacement, int driving_mode)
{
//	printf("displacement :  %d\n", displacement);
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


/*
int rectangular_coordinates_first_class_order(struct motor_list* robot_arm_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double* destination_position_x, double* destination_position_y, double* destination_position_z)
{
	if((*robot_arm_address).ractangular_macro_compliment_value == 'y')
	{
		*destination_position_x = purpose_position_x;
		*destination_position_y = purpose_position_y;
		*destination_position_z = purpose_position_z;
	}
}
*/

int combine_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, double purpose_time_out)
{

	//if(rectangular_coordinate_macroscopic_compliment_checking(robot_arm_address) == 'y')
	//{
		
		(*robot_arm_address).position_value.macroscopic_destination_position_x = purpose_position_x;
		(*robot_arm_address).position_value.macroscopic_destination_position_y = purpose_position_y;
		(*robot_arm_address).position_value.macroscopic_destination_position_z = purpose_position_z;
		(*function_name_address).macroscopic_destination_position_seta = purpose_seta;
		(*function_name_address).macroscopic_destination_position_grip = purpose_grip;
		(*robot_arm_address).time_out = purpose_time_out;
//	}

}

int function_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double seta, double grip)
{
	
}

int position_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address, double x, double y, double z)
{
	
}



int combine_macroscopic_to_micro_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	printf("check_3 %f\n", ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out));
	
	(*robot_arm_address).position_value.microscopic_destination_position_x = 
	(*robot_arm_address).position_value.macroscopic_current_position_x + ((*robot_arm_address).position_value.macroscopic_destination_position_x - (*robot_arm_address).position_value.macroscopic_current_position_x) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_y = 
	(*robot_arm_address).position_value.macroscopic_current_position_y + ((*robot_arm_address).position_value.macroscopic_destination_position_y - (*robot_arm_address).position_value.macroscopic_current_position_y) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	(*robot_arm_address).position_value.microscopic_destination_position_z = 
	(*robot_arm_address).position_value.macroscopic_current_position_z + ((*robot_arm_address).position_value.macroscopic_destination_position_z - (*robot_arm_address).position_value.macroscopic_current_position_z) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	
	//요기의 부분 설정에 따라, function part의 모터 제어를 func 내부의 position 구조체를 이용할 지, motor_mathod를 이용할지 선택할 수 있음. 
	(*function_name_address).microscopic_destination_position_seta = (*function_name_address).macroscopic_current_position_seta + ((*function_name_address).macroscopic_destination_position_seta - (*function_name_address).macroscopic_current_position_seta) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	(*function_name_address).microscopic_destination_position_grip = (*function_name_address).macroscopic_current_position_grip + ((*function_name_address).macroscopic_destination_position_grip - (*function_name_address).macroscopic_current_position_grip) * ((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	printf("check_7 \n x : %f, y : %f, z : %f\n", (*robot_arm_address).position_value.macroscopic_current_position_x, (*robot_arm_address).position_value.macroscopic_current_position_y, (*robot_arm_address).position_value.macroscopic_current_position_z);
	printf("checking sginal\n");
	printf("\n\ncheck 8 : %f, %f, %f,\n", 
			robot_arm_1.position_value.microscopic_destination_position_x, 
			robot_arm_1.position_value.microscopic_destination_position_y, 
			robot_arm_1.position_value.microscopic_destination_position_z
		 );
}


int function_part_position_transformation(double seta, double length, double x, double y, double z, double* address_of_x, double* address_of_y, double* address_of_z)
{
	
	printf("checking sginal\n");
	printf("\n\ncheck 9 : %f, %f, %f,\n", 
			robot_arm_1.position_value.microscopic_destination_position_x, 
			robot_arm_1.position_value.microscopic_destination_position_y, 
			robot_arm_1.position_value.microscopic_destination_position_z
	);

			 		 	
	//정관절인 경우만 적동하도록 제한 한다면. 
	if((seta-M_PI_2)*(seta-M_PI_2)>M_PI_2*M_PI_2)//범위에 같다가포함되면 func_seta의 범위가  0,인 경우와 M_PI인 경우에 에러 가 발생함 
	{
		return 0;//여기서 러턴 0이 발생하여 함수가 갑자기 종료됨. 특히나 처벗째에서 걸리는 경우 곤란?? 
	} 
	// 현재 함수의 경우  DEGREE_OF_SETA 가 atan2(y, x)를 통해 결정되는 경우 이므로 SETA의 값이 중요 하지 않음. 
	//이 아니리ㅏ role 각도가 있는 경우 이므로 atan2(y, x)와 개별 적인 변수 임. 
	double t_seta = (seta);
	*address_of_x = x - length*sin(t_seta)*cos(atan2(y, x));
	*address_of_y = y - length*sin(t_seta)*sin(atan2(y, x));
	*address_of_z = z - length*cos(t_seta);


	printf("\n\ncheck 10 : %f, %f, %f,\n", 
			x - length*sin(t_seta)*cos(atan2(y, x)),
			y - length*sin(t_seta)*sin(atan2(y, x)),
			z - length*cos(t_seta)
		 );
	
	printf("check 12 : x- : %f\ny- : %f\nz- : %f\n", *address_of_x, *address_of_y, *address_of_z);
	printf("check 14 : \n &x- : %x\n&y- : %x\n&z- : %x\n", address_of_x, address_of_y, address_of_z);
	//printf("check 16 : \n &x- : %x\n&y- : %x\n&z- : %x\n", xx, yy, zz);
	
}


int robot_4aix_arm_degree_clac(double length_1, double length_2, double length_3, 
double x_destination, double y_destination , double z_destination, double function_target_degree, double* m1_degree, double* m2_degree, double* m3_degree, double* m4_degree, double* cumulated_angle_position)
{
		printf("\n\ncheck 13 : %f, %f, %f,\n", 
			x_destination, 
			y_destination, 
			z_destination
		 );
	
	
	
	/*
	double r_0 = sqrt(x_destination*x_destination + y_destination*y_destination + z_destination*z_destination);
	
	double yaw_0 = atan2(y_destination, x_destination);
	//double pitch_0 = atan(z_destination/sqrt(x_destination*x_destination + y_destination*y_destination));//sholder
	double pitch_0 = 0;
	if(r_0 != 0)
	{
		pitch_0 = asin((z_destination/r_0));//sholder
	}
	else
	{
		pitch_0 = M_PI_2;
	}
	
	
	double phi_0 = M_PI_2-pitch_0;
	double side_1 = sqrt(length_1*length_1 + r_0*r_0 - 2*length_1*r_0*cos(phi_0));
	//printf("side_1 %f\n", side_1);
	double pitch_1 = asin((r_0 / side_1) * sin(phi_0));
	//double pitch_1 = asin((side_1 / r_0) * sin(phi_0));
	//ok
	
	double phi_1 = acos((side_1*side_1 + length_2*length_2 - length_3*length_3)/(2*side_1*length_2));
	//printf("phi_1 : %f\n", phi_1);
	double resultant_1 = M_PI - (phi_1 + pitch_1);
	double sigma = asin(side_1 / length_3 * sin(phi_1));
	//printf("sigma : %f\n", sigma);
	//double sigma = asin(length_3 / side_1 * sin(phi_1));
	double resultant_2 = M_PI - sigma;
	*/
	
	
	double r_0 = sqrt(x_destination*x_destination + y_destination*y_destination + z_destination*z_destination);
	double yaw_0 = atan2(y_destination, x_destination);
	double pitch_0 = atan(z_destination/sqrt(x_destination*x_destination + y_destination*y_destination));//sholder
	double phi_0 = M_PI_2-pitch_0;
	double side_1 = sqrt(length_1*length_1 + r_0*r_0 - 2*length_1*r_0*cos(phi_0));
	double pitch_1 = asin(r_0 / side_1 * sin(phi_0));
	double phi_1 = acos((side_1*side_1 + length_2*length_2 - length_3*length_3)/(2*side_1*length_2));
	double resultant_1 = M_PI - (phi_1 + pitch_1);
	double sigma = asin(side_1 / length_3 * sin(phi_1));
	double resultant_2 = M_PI - sigma;
//	printf("%f\n\n", (side_1*side_1 + length_2*length_2 - length_3*length_3)/2*side_1*length_2);
//	
//	printf("%f\n", r_0);
//	printf("%f\n", pitch_0);
//	printf("%f\n", phi_0);
//	printf("%f\n", side_1);
//	printf("%f\n", pitch_1);
//	printf("%f\n", phi_1);
//	printf("%f\n", resultant_1);
//	printf("%f\n", sigma);
//	printf("%f\n", resultant_2);
	*cumulated_angle_position = -(resultant_1 + resultant_2);
	*m1_degree = yaw_0;
	*m2_degree = resultant_1;
	*m3_degree = resultant_2;
	*m4_degree = -(resultant_1 + resultant_2);// + function_target_degree;
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

/*
int degree_to_step_2_arg(struct motor_list* robot_arm_address, int arm_number_of_motors, struct function_part* function_name_address, int arm_number_of_motors)
{
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	
	double rad_per_step = 2*M_PI/(double)(*motor_Ptr).step_per_revolution_presison;
	
	int i =0;
	while(i<arm_number_of_motors)
	{
		(*motor_Ptr).destination_step = (long)(((*motor_Ptr).destination_degree * (double)(*motor_Ptr).step_per_revolution_presison) / (2*M_PI));
		motor_Ptr++;
		i = i + 1;
	}
}
*/

void ordering_trans_form(struct motor_list* robot_arm_address, struct function_part* function_name_address)
{
	
	//combine_macroscopic_to_micro_ordering(struct motor_list* robot_arm_address, struct function_part* function_name_address);
	/*
	function_part_position_transformation((*function_name_address).microscopic_destination_position_seta, (*function_name_address).length, 
	(*robot_arm_address).position_value.microscopic_destination_position_x, (*robot_arm_address).position_value.microscopic_destination_position_y, (*robot_arm_address).position_value.microscopic_destination_position_z, 
	&((*robot_arm_address).position_value.transformed_destination_position_x), &((*robot_arm_address).position_value.transformed_destination_position_y), &((*robot_arm_address).position_value.transformed_destination_position_z)
	);
	*/
	
	function_part_position_transformation(
	(*function_name_address).microscopic_destination_position_seta, (*function_name_address).length, 
	(*robot_arm_address).position_value.microscopic_destination_position_x, (*robot_arm_address).position_value.microscopic_destination_position_y, (*robot_arm_address).position_value.microscopic_destination_position_z, 
	(&robot_arm_1.position_value.transformed_destination_position_x), (&robot_arm_1.position_value.transformed_destination_position_y), &(robot_arm_1.position_value.transformed_destination_position_z)
	);
	
	
	printf("\n\ncheck 11 : %f, %f, %f,\n", 
			robot_arm_1.position_value.transformed_destination_position_x, 
			robot_arm_1.position_value.transformed_destination_position_y, 
			robot_arm_1.position_value.transformed_destination_position_z
		 );
	
	printf("check 15 : \n&x- : %x\n&y- : %x\n&z- : %x\n\n&X- : %x\n&Y- : %x\n&Z- : %x\n", 
	
			&((*robot_arm_address).position_value.transformed_destination_position_x),
			&((*robot_arm_address).position_value.transformed_destination_position_y),
			&((*robot_arm_address).position_value.transformed_destination_position_z),
	
	
			&robot_arm_1.position_value.transformed_destination_position_x, 
			&robot_arm_1.position_value.transformed_destination_position_y, 
			&robot_arm_1.position_value.transformed_destination_position_z
			
			);
	
	robot_4aix_arm_degree_clac(
	
	(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3, 
	(*robot_arm_address).position_value.transformed_destination_position_x, (*robot_arm_address).position_value.transformed_destination_position_y, (*robot_arm_address).position_value.transformed_destination_position_z, 
	(*function_name_address).microscopic_destination_position_seta, // 사용중인 변 수 인지 확인 해 보기. 
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	
	);
	
	(*robot_arm_address).m5_servo_gripper.destination_degree = (*function_name_address).microscopic_destination_position_grip;
	
//	printf("check 5 : %f, %f, %f, %f, %f\n", robot_arm_1.m1_stepper.destination_degree/M_PI*180, robot_arm_1.m2_bldc.destination_degree/M_PI*180, robot_arm_1.m3_stepper.destination_degree/M_PI*180, robot_arm_1.m4_stepper.destination_degree/M_PI*180, robot_arm_1.m5_servo_gripper.destination_degree/M_PI*180);
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






int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list* robot_arm_address)
{
	//printf("%d", (*robot_arm_address).stream_of_time / (*robot_arm_address).time_out);
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	if((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out > 1)
	{
		(*robot_arm_address).position_value.macroscopic_current_position_x = (*robot_arm_address).position_value.microscopic_current_position_x;
		(*robot_arm_address).position_value.macroscopic_current_position_y = (*robot_arm_address).position_value.microscopic_current_position_y;
		(*robot_arm_address).position_value.macroscopic_current_position_z = (*robot_arm_address).position_value.microscopic_current_position_z;
		(*robot_arm_address).ractangular_macro_compliment_value = 'y';
		return 'y';
	}
	else
	{
		(*robot_arm_address).ractangular_macro_compliment_value = 'n';
		return 'n';
	}
}

int rectangular_coordinate_microscopic_compliment_checking(struct motor_list* robot_arm_address)
{
	//미시적 단계에서 어떻게 해결할까.....? .스텝 단위에서  현제위치와 완료 상태를 확인 하여야 할 듯 함.
	int n = 0;
	int m = 0;
	struct motor_mathod* motor_Ptr = (struct motor_mathod*)robot_arm_address;
	//printf("\n\n");
	while(n < 5)
	{
		//printf(" %d, %d/", (*motor_Ptr).current_step, (*motor_Ptr).destination_step);
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
	//printf("\n\n");
	if(m != 0)
	{
		return 'n';
	}
	else
	{
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

void general_motor_drivig(int motor_type, char motor_port, int motor_pin, int* current_hall_sensor_value, int* just_before_hall_sensor_value, int* direction_value, long* current_step_aderess)//, char hall_sensor_port, int hall_sensor_pin 등 홀센서 값읽기는 실시간 감시가 중요하기에 타이머 인터럽트가 대신 담당한다. 
{
	//이 함수에서 스텝모터/bldc, 강제 홀딩 여부, 포트및 핀설정, 방향 , 모든 것을 판단하여 말단 함수들에게 명령을내림.
	
	switch(motor_type)
	{
		case 'b':
		case 'B':
			//bldc_motor_driving_closed_loop(motor_port, motor_pin, current_hall_sensor_value, just_before_hall_sensor_value, direction_value);//파라메터를 받은 순간의 값과 함수가 실행중일 때의 값이 서로 다를 수 있기때문에 타이머 인터럽트로 계속 초기화 하는 값을 메모리 주소와 포인터를 이용하여 전달한다.
			break;
		case 's':
		case 'S':
			//step_motor_driving_open_loop(motor_port, motor_pin, direction_value, current_step_aderess);//딜레이 타임도 넣어러주면 섬세한 모터 제어에 좋을듯. 단 구조체에 미리 저장해두어 모터의 속성을 가져다 쓰듯이 사용하기. 
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


void microscopic_current_position_clac(struct motor_list* robot_arm_address, int number_of_arm_motor)
{
//	printf("=======================================\n");
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
	while(n < number_of_arm_motor-1)//3번 반복 실행할 것을 권장.
	{
//		printf("x : %f\ny : %f\nz : %f\n", x, y, z);
//		degree_pitch = degree_pitch + (*motor_Ptr).current_degree;
//		printf("current degree : %f\n", (*motor_Ptr).current_degree);
//		printf("degree_pitch : %f\n", degree_pitch);
//		printf("length %d : %f\n", n+1, (*length_Ptr));
//		printf("r_proj_xy %f \n", (*length_Ptr)*sin(degree_pitch));
		double r_proj_xy = (*length_Ptr)*sin(degree_pitch);
		x = x + r_proj_xy*cos(degree_yaw);
		y = y + r_proj_xy*sin(degree_yaw);
		z = z + (*length_Ptr)*cos(degree_pitch);
		
		n = n + 1;
		length_Ptr++;
		motor_Ptr++;
	}
	(*robot_arm_address).position_value.microscopic_current_position_x = x;
	(*robot_arm_address).position_value.microscopic_current_position_y = y;
	(*robot_arm_address).position_value.microscopic_current_position_z = z;
//	printf("x : %f\ny : %f\nz : %f\n", x, y, z);
//	printf("=======================================\n");
}




//void current_position_caliberation(struct motor_list* robot_arm_address)
//{
//	(*robot_arm_address).microscopic_current_position = 0;
//	(*motor_name).current_step = 0;
//	(*motor_name).initialization_value = 1;
//}

//수정필요 
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
	
	(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3, 
	(*robot_arm_address).position_value.transformed_current_position_x, (*robot_arm_address).position_value.transformed_current_position_y, (*robot_arm_address).position_value.transformed_current_position_z, 
	(*function_name_address).microscopic_destination_position_seta, // 사용중인 변 수 인지 확인 해 보기. 
	&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
	
	);
	
	(*robot_arm_address).m5_servo_gripper.destination_degree = purpose_grip;
	
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
	
	
	(*robot_arm_address).position_value.macroscopic_current_position_x = purpose_position_x;
	(*robot_arm_address).position_value.macroscopic_current_position_y = purpose_position_y;
	(*robot_arm_address).position_value.macroscopic_current_position_z = purpose_position_z;
	
	
	printf("\n현재 step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
	
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
	while(rectangular_coordinate_macroscopic_compliment_checking(&robot_arm_1) == 'n')
	{
		printf("c\n");
		combine_macroscopic_to_micro_ordering(&robot_arm_1, &func_gripper);
		printf("22222\n");
		printf("\n\ncheck 8 : %f, %f, %f,\n", 
			robot_arm_1.position_value.microscopic_destination_position_x, 
			robot_arm_1.position_value.microscopic_destination_position_y, 
			robot_arm_1.position_value.microscopic_destination_position_z
		 );
		
		ordering_trans_form(&robot_arm_1, &func_gripper); 
		
		printf("333333\n");
		printf("\n\ncheck 8 : %f, %f, %f,\n", 
			robot_arm_1.position_value.microscopic_destination_position_x, 
			robot_arm_1.position_value.microscopic_destination_position_y, 
			robot_arm_1.position_value.microscopic_destination_position_z
		 );
		
		//printf("\ncheck1 %d\n", rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1));
		while(rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1) == 'n')
		{
			
//			printf("\n\n%f, %f, %f,/\n %f, %f, %f\n", 
//			robot_arm_1.position_value.microscopic_destination_position_x, 
//			robot_arm_1.position_value.microscopic_destination_position_y, 
//			robot_arm_1.position_value.microscopic_destination_position_z, 
//			robot_arm_1.position_value.transformed_destination_position_x, 
//			robot_arm_1.position_value.transformed_destination_position_y, 
//			robot_arm_1.position_value.transformed_destination_position_z
//		 );
			//calc_derection_value(&robot_arm_1); 
			microscopic_to_driver_ordering(&robot_arm_1);
			motor_combo_serise_drivig(&robot_arm_1, 5);
			//printf("\n%d, %d\n\n", robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
			//printf("\n%f, %f\n\n", robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
//			printf("\n현재 step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
			//printf("미시목표  step %f, %f, %f, %f, %f\n", robot_arm_1.m1_stepper.destination_step, robot_arm_1.m2_bldc.destination_step, robot_arm_1.m3_stepper.destination_step, robot_arm_1.m4_stepper.destination_step, robot_arm_1.m5_servo_gripper.destination_step);
			microscopic_current_position_clac(&robot_arm_1, 4);
		}
		
		microscopic_current_position_clac(&robot_arm_1, 4); 
		
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
	
	motor_combo_calibration(&robot_arm_1, &func_gripper, 20, 0, 27, M_PI_2, 0, 5);//last argument is number_of_motors
	printf("\n현재 step %d, %d, %d, %d, %d\n\n", robot_arm_1.m1_stepper.current_step, robot_arm_1.m2_bldc.current_step, robot_arm_1.m3_stepper.current_step, robot_arm_1.m4_stepper.current_step, robot_arm_1.m5_servo_gripper.current_step);
	
	int robot_dirvig_compliment_value = 1;
//	current_position_caliberation(&robot_arm_1);
//	reset_trans_form(&robot_arm_1, &func_gripper); 
	
	printf("a");
	while(1)
	{
		combine_ordering(&robot_arm_1, &func_gripper, 30, 0, 27, M_PI_2, 255, 100);
		printf("b");
		whole_driving();
		break;
	}
} 













