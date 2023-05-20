#include<stdio.h>
#include<math.h>

//unit_scale : cm.
//������ ���� ������ y+(y�� + �������� ����)���� 

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
	//�������� ��츸 �����ϵ��� ���� �Ѵٸ�. 
	if((seta-M_PI_2)*(seta-M_PI_2)>M_PI_2*M_PI_2)
	{
		return 0;
	} 
	// ���� �Լ��� ���  DEGREE_OF_SETA �� atan2(y, x)�� ���� �����Ǵ� ��� �̹Ƿ� SETA�� ���� �߿� ���� ����. 
	//�� �ƴϸ��� role ������ �ִ� ��� �̹Ƿ� atan2(y, x)�� ���� ���� ���� ��. 
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


