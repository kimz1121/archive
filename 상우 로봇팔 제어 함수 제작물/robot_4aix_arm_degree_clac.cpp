#include<stdio.h>
#include<math.h>

//unit_scale : cm.

//동경의 각도 기준은 y+(y축 + 방향으로 평행)방향 
 


 

struct function_part

{

	double seta = M_PI_2;

	double length = 10;

}func_gripper;

struct robot_arm
{
	double l_1 = 27;
	double l_2 = 30;
	double l_3 = 30;
	
	
	double x_origin = 10;
	
	double y_origin = 0;

	double z_origin = 27;
	
	
	double x_destination = 0;
	
	double y_destination = 0;
	
	double z_destination = 0;
	
	
	
}robot_arm_x;





double m1 = 0;
double m2 = 0;
double m3 = 0;
double last_angle_position = 0;

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


struct motor_mathod
{
	double current_degree;
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
	
		struct position_management position_value;
	
}a;



void microscopic_current_position_clac(struct motor_list* robot_arm_address, int number_of_arm_motor)
{
	printf("=======================================\n");
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
		printf("x : %f\ny : %f\nz : %f\n", x, y, z);
		degree_pitch = degree_pitch + (*motor_Ptr).current_degree;
		printf("current degree : %f\n", (*motor_Ptr).current_degree);
		printf("degree_pitch : %f\n", degree_pitch);
		printf("length %d : %f\n", n+1, (*length_Ptr));
		printf("r_proj_xy %f \n", (*length_Ptr)*sin(degree_pitch));
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
	printf("x : %f\ny : %f\nz : %f\n", x, y, z);
	printf("=======================================\n");
}

int function_part_position_transformation(double seta, double length, double x, double y, double z, double* adress_of_x, double* adress_of_y, double* adress_of_z)
{
	//정관절인 경우만 적동하도록 제한 한다면. 
	if((seta-M_PI_2)*(seta-M_PI_2)>M_PI_2*M_PI_2)
	{
		return 0;
	} 
	// 현재 함수의 경우  DEGREE_OF_SETA 가 atan2(y, x)를 통해 결정되는 경우 이므로 SETA의 값이 중요 하지 않음. 
	//이 아니리ㅏ role 각도가 있는 경우 이므로 atan2(y, x)와 개별 적인 변수 임. 
	double t_seta = (seta);
	*adress_of_x = x - length*sin(t_seta)*cos(atan2(y, x));
	*adress_of_y = y - length*sin(t_seta)*sin(atan2(y, x));
	*adress_of_z = z - length*cos(t_seta);
}



int robot_4aix_arm_degree_clac(double l_1, double l_2, double l_3, double x_destination, double y_destination , double z_destination, double* m1_degree, double* m2_degree, double* m3_degree, double* cumulated_angle_position)
{
	double r_0 = sqrt(x_destination*x_destination + y_destination*y_destination + z_destination*z_destination);
	
	double yaw_0 = atan2(y_destination, x_destination);
	double pitch_0 = atan(z_destination/sqrt(x_destination*x_destination + y_destination*y_destination));//sholder
	double phi_0 = M_PI_2-pitch_0;
	double side_1 = sqrt(l_1*l_1 + r_0*r_0 - 2*l_1*r_0*cos(phi_0));
	double pitch_1 = asin(r_0 / side_1 * sin(phi_0));
	double phi_1 = acos((side_1*side_1 + l_2*l_2 - l_3*l_3)/(2*side_1*l_2));
	double resultant_1 = M_PI - (phi_1 + pitch_1);
	double sigma = asin(side_1 / l_3 * sin(phi_1));
	double resultant_2 = M_PI - sigma;
	printf("%f\n\n", (side_1*side_1 + l_2*l_2 - l_3*l_3)/2*side_1*l_2);
	
	printf("%f\n", r_0);
	printf("%f\n", pitch_0);
	printf("%f\n", phi_0);
	printf("%f\n", side_1);
	printf("%f\n", pitch_1);
	printf("%f\n", phi_1);
	printf("%f\n", resultant_1);
	printf("%f\n", sigma);
	printf("%f\n", resultant_2);
	*cumulated_angle_position = -(resultant_1 + resultant_2);
	*m1_degree = yaw_0;
	*m2_degree = resultant_1;
	*m3_degree = resultant_2;
}



int main()
{
	function_part_position_transformation(func_gripper.seta, func_gripper.length, robot_arm_x.x_origin, robot_arm_x.y_origin, robot_arm_x.z_origin, &robot_arm_x.x_destination, &robot_arm_x.y_destination, &robot_arm_x.z_destination);
	robot_4aix_arm_degree_clac(robot_arm_x.l_1, robot_arm_x.l_2, robot_arm_x.l_3, robot_arm_x.x_destination, robot_arm_x.y_destination, robot_arm_x.z_destination, &m1, &m2, &m3, &last_angle_position);
	printf("\n\n");
	printf("%lf, %lf, %lf, %lf", m1/M_PI*180, m2/M_PI*180, m3/M_PI*180, last_angle_position/M_PI*180);
}
