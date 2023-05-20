#include<stdio.h>
#include<math.h>

//unit_scale : cm.
//동경의 각도 기준은 y+(y축 + 방향으로 평행)방향 

double x_origin = 40;
double y_origin = 0;
double z_origin = 30;

double x_destination = 0;
double y_destination = 0;
double z_destination = 0;

struct function_part
{
	double seta = M_PI_2;
	double length = 5;
}func_gripper;



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

int main()
{
	function_part_position_transformation(func_gripper.seta, func_gripper.length, x_origin, y_origin, z_origin, &x_destination, &y_destination, &z_destination);
	printf("%f %f %f\n", x_destination, y_destination, z_destination);
}


