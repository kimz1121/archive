/*
file name: binary_file_editor.cpp
created: 2021.01.18
modified: 2021.01.30
author: kimz1121, ekg1229 
*/
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <stdlib.h>
// #include <windows.h>

using namespace std;

struct file_header // 비트맵 파일 헤더
{
	uint32_t bfType;	  // BMP 파일 매직 넘버
	uint32_t bfSize;	  // 바이트 단위로 전체 파일 크기
	uint32_t bfReserved1; // 예약된 변수
	uint32_t bfReserved2; // 예약된 변수
	uint32_t bfOffBits;	  // 비트맵 데이터의 시작 위치
};

struct info_header // 비트맵 정보 헤더
{

	uint32_t biSize;		  // 현재 비트맵 정보 헤더의 크기
	uint32_t biWidth;		  // 픽셀단위로 영상의 폭
	uint32_t biHeight;		  // 영상의 높이
	uint32_t biplanes;		  // 비트 플레인 수 (항상 1)
	uint32_t biBitCount;	  // 픽셀당 비트수 (컬러, 흑백 구별)
	uint32_t biCompression;	  // 압축 유무
	uint32_t biSizeImage;	  // 영상의 크기 (바이트 단위)
	uint32_t biXPelsPerMeter; // 가로 해상도
	uint32_t biYPelsPerMeter; // 세로 해상도
	uint32_t biClrUsed;		  // 실제 사용 색상 수
	uint32_t biClrImportant;  // 중요한 색상 인덱스
};

class robot()
{
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

	} func_gripper;

	struct motor_mathod
	{
		//바뀌기 어려운 기계고유적 특성
		int motor_type = 'v';			  //'s' step,'b' bldc
		int step_per_revolution_presison; //임시값 100.

		//기계고유적 특성과 사용자의 임의 지정의 사이
		char proper_direction;
		char initialization_value;

		//사용자가 임의로 설치한 특성
		char motor_port;
		int motor_pin;
		int hall_sensor_pin;

		//기계부품 제어 특성
		char motor_driving_mode; //stop or natural.

		//하드웨어적 특성을 실시간으로 저장하기 위한 변수
		//general, 공통의
		char error;
		int cumulated_steps;

		//hall_sensor_only, 홀센서 전용
		int current_hall_sensor_value;	   // = -1
		int just_before_hall_sensor_value; // = -1

		//모터 제어 및 구동 관련.
		int microscopic_displacement_value;
		char direction_value;

		double progress_situation;	 // = 0//진행상황
		char macroscopic_complition; // = 0//standard 0 , present progressive -1, 1 finished.
		char microscopic_complition; // = 0//standard 0 , present progressive -1, 1 finished.

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
		struct motor_mathod m1_stepper; //제 1관절 모터
		struct motor_mathod m2_stepper; //제 2관절 모터_step_motor_mode
		//struct motor_mathod m2_bldc;//제 2관절 모터
		struct motor_mathod m3_stepper; //제 3관절 모터
		struct motor_mathod m4_stepper; //제 4관절 모터
		struct motor_mathod m5_servo_gripper;
		void *ending_NULL_pointer; // = NULL

		double length_1 = 26;
		double length_2 = 30;
		double length_3 = 30;
		double length_4_func_part = 10;

		double stream_of_time;
		double time_out;

		char ractangular_macro_compliment_value; //'y' is done, 'n' is on progress.
		char ractangular_micro_compliment_value;

		struct position_management position_value;

		double cumulated_angle_position;

		int number_of_motors;

	} robot_arm_1;
	//ending_NULL_pointer = NULL;

	//<가상 및 테스트용 함수 정리>77777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
	//가상 위치 계산 함수
	//<시작>========================================
	void virtual_position_management(long *current_position, int direction_value) //use it with delay.
	{
		switch (direction_value)
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

	//<경로계산 관련 함수>88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
	//<시작>========================================
	//어깨부분과 팔 부분을 구분하여 계산을 나누기.
	//팔이 가져야할 기구적 형태는 3차원 적 길이에 따라 나타남. 절대적인 위치에 대해서는 상대적으로 자유로운 편
	//길이와 관절 개수 만으로 기계관절의 형태를 정할 수 있나??
	//<끝>========================================
	//<시작>========================================
	int current_time_managemant() //현재 쓰이지 않음.
	{
	}
	//<끝>========================================
	//<시작>========================================
	//필요한 함수 원형 선언
	int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list * robot_arm_address, struct function_part * function_name_address);

	void combine_ordering(struct motor_list * robot_arm_address, struct function_part * function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, double purpose_time_out)
	{
		if (rectangular_coordinate_macroscopic_compliment_checking(robot_arm_address, function_name_address) == 'y')
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
	int function_ordering(struct motor_list * robot_arm_address, struct function_part * function_name_address, double seta, double grip)
	{
	}
	//<끝>========================================
	//<시작>========================================
	int position_ordering(struct motor_list * robot_arm_address, struct function_part * function_name_address, double x, double y, double z)
	{
	}
	//<끝>========================================
	//<시작>========================================
	void combine_macroscopic_to_micro_ordering(struct motor_list * robot_arm_address, struct function_part * function_name_address)
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
	int function_part_position_transformation(double seta, double length, double x, double y, double z, double *address_of_x, double *address_of_y, double *address_of_z)
	{
		//정관절인 경우만 적동하도록 제한 한다면.

		if ((seta - M_PI_2) * (seta - M_PI_2) > M_PI_2 * M_PI_2) //범위에 같다가포함되면 func_seta의 범위가  0,인 경우와 M_PI인 경우에 에러 가 발생함
		{
			return 0; //여기서 러턴 0이 발생하여 함수가 갑자기 종료됨. 특히나 처벗째에서 걸리는 경우 곤란??
		}
		// 현재 함수의 경우  DEGREE_OF_SETA 가 atan2(y, x)를 통해 결정되는 경우 이므로 SETA의 값이 중요 하지 않음.
		//이 아니리ㅏ role 각도가 있는 경우 이므로 atan2(y, x)와 개별 적인 변수 임.
		double atan_value = 0;
		if (x == 0, y == 0)
		{
			atan_value = 0;
		}
		else
		{
			atan_value = atan2(y, x);
		}

		double sin_value = 0;
		if (seta == M_PI)
		{
			sin_value = 0;
		}
		else
		{
			sin_value = sin(seta);
		}

		double cos_value = 0;
		if (seta - (long)(seta / M_PI) == M_PI_2)
		{
			cos_value = 1;
		}
		else
		{
			cos_value = cos(seta);
		}

		*address_of_x = x - length * sin_value * cos(atan_value);
		*address_of_y = y - length * sin_value * sin(atan_value);
		*address_of_z = z - length * cos(seta);
	}
	//<끝>========================================
	//<시작>========================================
	int robot_4aix_arm_degree_clac(struct motor_list * robot_arm_address, double length_1, double length_2, double length_3,
								   double x_destination, double y_destination, double z_destination, double function_target_degree, double *m1_degree, double *m2_degree, double *m3_degree, double *m4_degree, double *cumulated_angle_position)
	{

		double r_0 = sqrt(x_destination * x_destination + y_destination * y_destination + z_destination * z_destination);

		double yaw_0 = atan2(y_destination, x_destination);

		double pitch_0 = atan2(z_destination, sqrt(x_destination * x_destination + y_destination * y_destination)); //sholder
		double phi_0 = M_PI_2 - pitch_0;
		double side_1 = sqrt(length_1 * length_1 + r_0 * r_0 - 2 * length_1 * r_0 * cos(phi_0));

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
		if (
			((*robot_arm_address).position_value.transformed_current_position_x == 0 && (*robot_arm_address).position_value.transformed_current_position_y == 0 && (*robot_arm_address).position_value.transformed_current_position_z - length_1 == 0) || ((*robot_arm_address).position_value.transformed_destination_position_x == 0 && (*robot_arm_address).position_value.transformed_destination_position_y == 0 && (*robot_arm_address).position_value.transformed_destination_position_z - length_1 == 0))
		{
			double r_proj_xy_0 = sqrt((*robot_arm_address).position_value.transformed_current_position_x * (*robot_arm_address).position_value.transformed_current_position_x + (*robot_arm_address).position_value.transformed_current_position_y * (*robot_arm_address).position_value.transformed_current_position_y);
			double r_proj_xy_1 = sqrt((*robot_arm_address).position_value.transformed_destination_position_x * (*robot_arm_address).position_value.transformed_destination_position_x + (*robot_arm_address).position_value.transformed_destination_position_y * (*robot_arm_address).position_value.transformed_destination_position_y);

			double delta_r_proj_xy = r_proj_xy_1 - r_proj_xy_0;

			double delta_z = (*robot_arm_address).position_value.transformed_destination_position_z - (*robot_arm_address).position_value.transformed_current_position_z;

			if (((*robot_arm_address).position_value.transformed_current_position_x == 0 && (*robot_arm_address).position_value.transformed_current_position_y == 0 && (*robot_arm_address).position_value.transformed_current_position_z - length_1 == 0))
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
			if (check_value > 1)
			{
				check_value = 1;
			}
			pitch_1 = asin(check_value);
			phi_1 = acos((side_1 * side_1 + length_2 * length_2 - length_3 * length_3) / (2 * side_1 * length_2));
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
	//<끝>========================================
	//<시작>========================================
	void ordering_trans_form(struct motor_list * robot_arm_address, struct function_part * function_name_address)
	{
		// 입력된 수중 불필요한 소수부분을 제거하거나, 무리수를 입력 받았을시 오류를 발생시키는 함수.
		function_part_position_transformation(
			(*function_name_address).microscopic_destination_position_seta, (*function_name_address).length,
			(*robot_arm_address).position_value.microscopic_destination_position_x, (*robot_arm_address).position_value.microscopic_destination_position_y, (*robot_arm_address).position_value.microscopic_destination_position_z,
			(&robot_arm_1.position_value.transformed_destination_position_x), (&robot_arm_1.position_value.transformed_destination_position_y), &(robot_arm_1.position_value.transformed_destination_position_z));

		robot_4aix_arm_degree_clac(
			robot_arm_address,
			(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3,
			(*robot_arm_address).position_value.transformed_destination_position_x, (*robot_arm_address).position_value.transformed_destination_position_y, (*robot_arm_address).position_value.transformed_destination_position_z,
			(*function_name_address).microscopic_destination_position_seta, // 사용중인 변 수 인지 확인 해 보기.
			//m2_bldc_mode
			//&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
			//m2_step_motor_mode
			&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_stepper.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position));

		(*robot_arm_address).m5_servo_gripper.destination_degree = (*function_name_address).microscopic_destination_position_grip;

		// degree_to_step(&robot_arm_1, 5);
	}
	//<끝>========================================
	//<시작>========================================
	//<끝>========================================

	//<동작완료 점검 함수>BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
	//<시작>========================================
	int rectangular_coordinate_macroscopic_compliment_checking(struct motor_list * robot_arm_address, struct function_part * function_name_address)
	{

		//printf("%c\n", (*robot_arm_address).ractangular_macro_compliment_value);
		if ((*robot_arm_address).ractangular_macro_compliment_value == 'n')
		{
			if (((*robot_arm_address).stream_of_time / (*robot_arm_address).time_out) > 1)
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
		if ((*robot_arm_address).ractangular_macro_compliment_value == 'y')
		{
			//time_initialze
			(*robot_arm_address).stream_of_time = 0;
			return 'y';
		}
	}

	void virtual_rectangular_coordinate_microscopic_compliment_checking(struct motor_list *robot_arm_address, struct function_part *function_name_address)
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
}


	//<끝>========================================

	//<위치 초기화>CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
	//<시작>========================================
	void motor_combo_calibration(struct motor_list * robot_arm_address, struct function_part * function_name_address, double purpose_position_x, double purpose_position_y, double purpose_position_z, double purpose_seta, double purpose_grip, int number_of_motor)
	{
		//	combine_ordering(robot_arm_address, function_name_address, purpose_position_x, purpose_position_y, purpose_position_z, purpose_seta, purpose_grip, 1);
		//	(*robot_arm_address).stream_of_time = 1;
		//	ordering_trans_form(robot_arm_address, function_name_address);

		function_part_position_transformation(purpose_seta, (*function_name_address).length,
											  purpose_position_x, purpose_position_y, purpose_position_z,
											  &(*robot_arm_address).position_value.transformed_current_position_x, &(*robot_arm_address).position_value.transformed_current_position_y, &(*robot_arm_address).position_value.transformed_current_position_z);

		robot_4aix_arm_degree_clac(
			robot_arm_address,
			(*robot_arm_address).length_1, (*robot_arm_address).length_2, (*robot_arm_address).length_3,
			(*robot_arm_address).position_value.transformed_current_position_x, (*robot_arm_address).position_value.transformed_current_position_y, (*robot_arm_address).position_value.transformed_current_position_z,
			purpose_seta, // 사용중인 변 수 인지 확인 해 보기.
			//m2_bldc_mode
			//&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_bldc.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position)
			//m2_step_motor_mode
			&((*robot_arm_address).m1_stepper.destination_degree), &((*robot_arm_address).m2_stepper.destination_degree), &((*robot_arm_address).m3_stepper.destination_degree), &((*robot_arm_address).m4_stepper.destination_degree), &((*robot_arm_address).cumulated_angle_position));

		// degree_to_step(&robot_arm_1, 5);

		// struct motor_mathod *motor_Ptr = (struct motor_mathod *)robot_arm_address;

		// int n = 0;

		// while (n < number_of_motor)
		// {
		// 	(*motor_Ptr).current_step = (*motor_Ptr).destination_step;
		// 	motor_Ptr++;
		// 	n = n + 1;
		// }

		//current_position_organizing.

		(*robot_arm_address).position_value.macroscopic_current_position_x = purpose_position_x;
		(*robot_arm_address).position_value.macroscopic_current_position_y = purpose_position_y;
		(*robot_arm_address).position_value.macroscopic_current_position_z = purpose_position_z;
		(*function_name_address).macroscopic_current_position_seta = purpose_seta;
		(*robot_arm_address).m5_servo_gripper.destination_degree = purpose_grip;
		(*robot_arm_address).stream_of_time = 0;
	}
	//<끝>========================================

	double position[5][3] = {
		0,
	};

	void position_calc_for_each_joint(struct motor_list * robot_arm_address, double arg_position[][3])
	{
		// struct motor_mathod *motor_Ptr = (struct motor_mathod *)robot_arm_address;

		double leg_1 = robot_arm_address->length_1;
		double leg_2 = robot_arm_address->length_2;
		double leg_3 = robot_arm_address->length_3;
		double leg_4 = robot_arm_address->length_4_func_part;

		// double deg_1 = (*motor_Ptr++).current_degree;
		// double deg_2 = (*motor_Ptr++).current_degree;
		// double deg_3 = (*motor_Ptr++).current_degree;
		// double deg_4 = (*motor_Ptr++).current_degree;

		double deg_1 = (*robot_arm_address).m1_stepper.destination_degree;
		double deg_2 = (*robot_arm_address).m2_stepper.destination_degree;
		double deg_3 = (*robot_arm_address).m3_stepper.destination_degree;
		double deg_4 = (*robot_arm_address).m4_stepper.destination_degree;

		arg_position[0][0] = 0;
		arg_position[0][1] = 0;
		arg_position[0][2] = 0;

		arg_position[1][0] = 0;
		arg_position[1][1] = 0;
		arg_position[1][2] = leg_1;

		arg_position[2][0] = leg_2 * cos(deg_1) * cos(M_PI_2 - deg_2);
		arg_position[2][1] = leg_2 * sin(deg_1) * cos(M_PI_2 - deg_2);
		arg_position[2][2] = leg_1 + leg_2 * sin(M_PI_2 - deg_2);

		arg_position[3][0] = leg_2 * cos(deg_1) * cos(M_PI_2 - deg_2) + leg_3 * cos(deg_1) * cos(M_PI_2 - (deg_2 + deg_3));
		arg_position[3][1] = leg_2 * sin(deg_1) * cos(M_PI_2 - deg_2) + leg_3 * sin(deg_1) * cos(M_PI_2 - (deg_2 + deg_3));
		arg_position[3][2] = leg_1 + leg_2 * sin(M_PI_2 - deg_2) + leg_3 * sin(M_PI_2 - (deg_2 + deg_3));

		arg_position[4][0] = leg_2 * cos(deg_1) * cos(M_PI_2 - deg_2) + leg_3 * cos(deg_1) * cos(M_PI_2 - (deg_2 + deg_3)) + leg_4 * cos(deg_1) * cos(M_PI_2 - (deg_2 + deg_3 + deg_4));
		arg_position[4][1] = leg_2 * sin(deg_1) * cos(M_PI_2 - deg_2) + leg_3 * sin(deg_1) * cos(M_PI_2 - (deg_2 + deg_3)) + leg_4 * cos(deg_1) * cos(M_PI_2 - (deg_2 + deg_3 + deg_4));
		arg_position[4][2] = leg_1 + leg_2 * sin(M_PI_2 - deg_2) + leg_3 * sin(M_PI_2 - (deg_2 + deg_3)) + leg_4 * sin(M_PI_2 - (deg_2 + deg_3 + deg_4));
	}

	void draw_robot()
	{

	}

	void virtual_driving() //while문에 넣어서 사용하기
	{
		//나중에는 이상적 로봇 위치와 모터의 각도 해상도를 고려한 모델간의 변위를 측정 해 볼 수도 있을듯
		//그리기함수가 읽어야 할 값은 미시적 각도.
		// motor_mathod ->
		/*
	double current_degree = 0;
	double destination_degree = 0;
	*/
		if(rectangular_coordinate_macroscopic_compliment_checking(&robot_arm_1, &func_gripper) == 'n')
		{
			cout << robot_arm_1.stream_of_time << endl;

			cout << pos[4][0] << endl;

			cout << robot_arm_1.position_value.microscopic_destination_position_x << endl;

			combine_macroscopic_to_micro_ordering(&robot_arm_1, &func_gripper); //거시적 위치와 시간 함수에만 의존. //거시적 위치는 combine_ordering() 함수와 rectangular_coordinate_macroscopic_compliment_checking() 함수에 의해서만 변화함...
			ordering_trans_form(&robot_arm_1, &func_gripper);					//미시적 위치를 계산하여 기구학적 각도로 전환하는 함수 계산하는 함수.
			virtual_rectangular_coordinate_microscopic_compliment_checking(&robot_arm_1, &func_gripper);
			position_calc_for_each_joint(&robot_arm_1, position);
			robot_arm_1.stream_of_time = robot_arm_1.stream_of_time + 1;
		}
	}
	//=====================================
}robot_sim;

struct layer
{
	/*
		마스크 사용 규칙 정리

		1.
		마스크는 최종용 1개와 과 레이어 용 래이어 마다 2개씩 가지며,
		레이어 인스턴스 복제에따라서 레이어용 마스크의 수는 가변적이다.

		2.
		마스크는 2차원 배열로 구현한다.
		즉, 실체는 더블 포인터와 같다.
	*/

	/*

		레이어 구조체 설명

		1. 레이어 구조체의 역할
		-필요한 데이터를 생성 시와 소멸 시 자동으로 할당하고 해제한다.
		-레이어 범위에서의 그리기 영역선택을 한다.
		-그리기를 했던 영역으로 써의 자취를 통해 그려진 형상을 저장한다.
		-그리기 한 형상의 색을 저장한다. 
		-레이어를 최종 저장본에 적용할지 말지 레이커 켜고 끄기를 통해 선택할 수 있다.
		
		-없는 색은 아직 구현하지 않았으나 색도 형상도 없는 것으로 구현하는 방법이 있다.

		2. 레이어 구조체의 구조
		
		변수
		
		#2개의 마스크
		-레이어 영역 마스크
		>dptr_mask_area[x][y]
		-레이어 형상 마스크
		>dptr_mask_shape[x][y]
		
		#2개의 정보
		-색상 정보
		>tptr_data_coulor[x][y][RGB]
		-끄고 켜기 정보
		>data_on_off

		#3개의 전달 맴버 변수
		>member_width
		>member_height
		>member_byte_for_pixel

		함수
		-생성자
		-소멸자
		-적재
		-저장
		-초기화
		-영역 선택
		-1 픽셀단위 쓰기(색 및 형상 편집)
		-1 픽셀단위 지우기
		

		3. 레이어 구조체의 사용
		-레이어 구조체는 기본적으로 다른 클래스에 맴버로써 복제하여 사용한다.
		-레이어에 버퍼의 데이터를 적재하여 사용한다. 사용 후 적재 명령어로 버퍼로 덮어쓰기하여 데이터를 출력한다. 
		-편집은 1픽셀단위로만 가능함으로 쓰기 함수를 무수히 여러번 호출하여 사용한다. 그러나 동시에 여러개를 호출하는 것은 오버플로를 일으킬 수 있음으로 지양한다.
		-적재시에는 항상 사이즈가 충분한 알맞은 버퍼를 사용한다. 

		4. 작성자의 규칙

		dptr 접두사로 시작하는 변수는 더블 포인터 이다.
		tptr 접두사로 시작하는 변수는 트리플 포인터 이다.
		member 접두사로 시작하는 변수는 editor 객체로 부터 전달 받지만 객체가 종속되지 않고도 고유하게 동작하도록 중복하여 선언한 변수이다.

		-일단 위처럼 계획하구 있어요! 다른 의견 있으면 말씀해 주세요!

	*/

	bool **dptr_mask_area = nullptr; //마스크 행렬이 부여한느 방법이 있고 영역이 항상 사각형이라면 좌표로 부여하는 방법도 있다....차후 브러시를 이용한 영역선택과 같은 고급 기능을 위해 행렬로 부여하자.
	bool **dptr_mask_shape = nullptr;
	uint8_t ***tptr_data_coulor = nullptr;
	bool data_on_off = -1;

	int member_width = 0;
	int member_height = 0;
	int member_byte_for_pixel = 0;

	layer(int width, int height, int byte_for_pixel)
	{
		layer::member_width = width;
		layer::member_height = height;
		layer::member_byte_for_pixel = byte_for_pixel;

		dptr_mask_area = new bool *[width];
		dptr_mask_shape = new bool *[width];
		tptr_data_coulor = new uint8_t **[width];

		for (int i = 0; i < width; i++) //반복문을 통한 동적할당
		{
			dptr_mask_area[i] = new bool[height];
			dptr_mask_shape[i] = new bool[height];
			tptr_data_coulor[i] = new uint8_t *[height];

			for (int j = 0; j < height; j++)
			{
				tptr_data_coulor[i][j] = new uint8_t[byte_for_pixel];
			}
		}
		clear(); //값을 초기화해줌
	}

	~layer() //소멸자
	{
		for (int i = 0; i < member_height; i++)
		{
			for (int j = 0; j < member_byte_for_pixel; j++)
			{
				delete tptr_data_coulor[i][j];
			}
			delete dptr_mask_area[i];
			delete dptr_mask_shape[i];
			delete tptr_data_coulor[i];
		}
		delete dptr_mask_area;
		delete dptr_mask_shape;
		delete tptr_data_coulor;

		bool **dptr_mask_area = nullptr;
		bool **dptr_mask_shape = nullptr;
		uint8_t ***tptr_data_coulor = nullptr;
	}

	void area(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y) //그림을 그릴 영역 지정
	{
		if (pos_1_x > pos_2_x)
		{
			pos_1_x = pos_1_x ^ pos_2_x;
			pos_2_x = pos_1_x ^ pos_2_x;
			pos_1_x = pos_1_x ^ pos_2_x;
		}

		if (pos_1_y > pos_2_y)
		{
			pos_1_y = pos_1_y ^ pos_2_y;
			pos_2_y = pos_1_y ^ pos_2_y;
			pos_1_y = pos_1_y ^ pos_2_y;
		}
		//xor swap

		if (pos_1_x < 0)
		{
			pos_1_x = 0;
		}
		if (pos_1_y < 0)
		{
			pos_1_y = 0;
		}
		if (pos_2_x >= member_width)
		{
			pos_2_x = member_width - 1;
		}
		if (pos_2_y >= member_height)
		{
			pos_2_y = member_height - 1;
		}

		for (int i = pos_1_x; i <= pos_2_x; i++)
		{
			for (int j = pos_1_y; j <= pos_2_y; j++)
			{
				dptr_mask_area[i][j] = 1; //영역 마스크 true, 나머지 0
			}
		}
	}

	void area_set_total(bool Boolean)
	{
		for (int i = 0; i < member_width; i++)
		{
			for (int j = 0; j < member_height; j++)
			{
				dptr_mask_area[i][j] = Boolean; // 값을 하나 받아서 전체를 0으로 할 것인지, 1로 할 것인지
			}
		}
	}

	void clear()
	{
		for (int i = 0; i < member_width; i++)
		{
			for (int j = 0; j < member_height; j++)
			{
				dptr_mask_area[i][j] = 0;
				dptr_mask_shape[i][j] = 0; //shape(그림 형태)
				tptr_data_coulor[i][j][0] = 0;
				tptr_data_coulor[i][j][1] = 0;
				tptr_data_coulor[i][j][2] = 0;
			}
		}
	}

	void pixel(int pos_x, int pos_y, bool draw, uint8_t R, uint8_t G, uint8_t B) //위치, 값이 1이면 색을 바꿈. 값이 0이면 색이 없게.
	{
		if (pos_x >= 0 && pos_x < member_width && pos_y >= 0 && pos_y < member_height)
		{
			if (draw) //쓰기 상태
			{
				// cout<<"pixel_test_1"<<endl;
				dptr_mask_shape[pos_x][pos_y] = 1;
				// cout<<"pixel_test_2"<<endl;
				tptr_data_coulor[pos_x][pos_y][0] = B;
				// cout<<"pixel_test_3"<<endl;
				tptr_data_coulor[pos_x][pos_y][1] = G;
				// cout<<"pixel_test_4"<<endl;
				tptr_data_coulor[pos_x][pos_y][2] = R;
				// cout<<"pixel_test_5"<<endl;
			}
			else //지우기 상태
			{
				dptr_mask_shape[pos_x][pos_y] = 0;
				tptr_data_coulor[pos_x][pos_y][0] = 0;
				tptr_data_coulor[pos_x][pos_y][1] = 0;
				tptr_data_coulor[pos_x][pos_y][2] = 0;
			}
		}
	}

	void load()
	{
	}

	void store(char *refined_data_buff, int buff_width, int buff_height) //지정한 버퍼의 크기만큼 픽셀 복사하기.
	{
		for (int y = 0; y < member_height; y++)
		{
			for (int x = 0; x < member_width; x++)
			{
				if (dptr_mask_shape[x][y] && dptr_mask_area[x][y]) //dptr_mask_shape와 dptr_mask_area가 둘다 true일 때
				{
					*(refined_data_buff + (member_byte_for_pixel * x) + (member_byte_for_pixel * buff_width * (member_height - (y + 1))) + 0) = tptr_data_coulor[x][y][0];
					*(refined_data_buff + (member_byte_for_pixel * x) + (member_byte_for_pixel * buff_width * (member_height - (y + 1))) + member_byte_for_pixel / 3) = tptr_data_coulor[x][y][1];
					*(refined_data_buff + (member_byte_for_pixel * x) + (member_byte_for_pixel * buff_width * (member_height - (y + 1))) + (member_byte_for_pixel / 3) * 2) = tptr_data_coulor[x][y][2];
				}
			} //이 부분의 대입에서 x와 y의 좌표값을 반전하여 대입하면 레이어 단위로 반전 시키는 것이 가능함.
		}
	}
};

class bmp_editor
{
public:
	ifstream f_in_image;
	ofstream f_out_image;

	char file_head[5] = {2, 4, 2, 2, 4}; // 단 하나만 존재하면 되는 전역 변수로는 어떻게 선언할까?
	char info_head[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};

	int size_of_file = 0;
	const int size_of_header = 54;
	int pure_size_of_data = 0;

	char *rare_buff = NULL;

	char *head_buff = NULL;
	char *package_data_buff = NULL;
	char *reverse_data_buff = NULL;

	char *edit_head_buff = NULL;

	char *edit_refined_data_buff = NULL;
	char *edit_refined_data_copy_buff = NULL;

	// (*buff_ptr) = temp_buff;
	// buff_ptr++;

	bool **dptr_mask_area_editor_final = nullptr;

public:
	struct layer *layer_arr[5] = {
		nullptr,
	}; // 차후 링크드 리스트로 자료의 양을 동적으로 조절하면 좋을 듯

	struct file_header bitmap_FILE_HEADER = {0, 0, 0, 0, 0};				   //file_header 0으로 초기화
	struct info_header bitmap_INFO_HEADER = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //info_header 0으로 초기화

	int image_width = 0;
	int image_height = 0;

	bmp_editor(const char *f_in_name)
	{
		cout << "에디터 생성자 0" << endl;
		open_file(f_in_name, 'I');
		cout << "에디터 생성자 1" << endl;
		head_buff = new char[size_of_header];
		edit_head_buff = new char[size_of_header];

		read_header_bmp();
		cout << "에디터 생성자 2" << endl;

		edit_head_load();
		cout << "에디터 생성자 3" << endl;

		size_of_file = bitmap_FILE_HEADER.bfSize;
		rare_buff = new char[size_of_file];
		cout << "에디터 생성자 4" << endl;

		f_in_image.seekg(0, ios::beg); //이게 없었을 때 read_header_bmp(); 함수에서 읽고 되돌려 놓지 않은 파일 포인터 길이 만큼 출력이미지에 공백이 발생하였음
		f_in_image.read(rare_buff, size_of_file);
		// edit_head_buff = new char(size_of_header);//실수 박제
		cout << "에디터 생성자 5" << endl;
		image_width = bitmap_INFO_HEADER.biWidth;
		image_height = bitmap_INFO_HEADER.biHeight;

		package_data_buff = new char[bitmap_INFO_HEADER.biSizeImage]; //공간 할당
		reverse_data_buff = new char[bitmap_INFO_HEADER.biSizeImage];

		pure_size_of_data = image_width * image_height * (bitmap_INFO_HEADER.biBitCount / 8); //데이터의 픽셀 수(폭*높이/8)

		edit_refined_data_buff = new char[pure_size_of_data];
		edit_refined_data_copy_buff = new char[pure_size_of_data];

		cout << bitmap_INFO_HEADER.biSizeImage << endl;
		cout << (bitmap_INFO_HEADER.biBitCount / 8) << endl;
		cout << "에디터 생성자 6" << endl;
		read_data_bmp(); //데이터 읽어오기
		cout << "에디터 생성자 7" << endl;
		edit_data_load();

		dptr_mask_area_editor_final = new bool *[image_width];

		for (int i = 0; i < image_width; i++)
		{
			dptr_mask_area_editor_final[i] = new bool[image_height];
		}

		layer_arr[0] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount / 8); //*(layer_arr[0])바로 사용 가능.
		layer_arr[1] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount / 8);
		layer_arr[2] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount / 8);
		layer_arr[3] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount / 8);
		layer_arr[4] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount / 8);
		// layer_arr[5] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[6] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[7] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[8] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[9] = new layer(image_width, image_height, bitmap_INFO_HEADER.biBitCount/8);

		cout << "에디터 생성자 동작 완료" << endl;
	}

	void area_editor(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y) //선택 영역 지정
	{
		if (pos_1_x > pos_2_x)
		{
			pos_1_x = pos_1_x ^ pos_2_x;
			pos_2_x = pos_1_x ^ pos_2_x;
			pos_1_x = pos_1_x ^ pos_2_x;
		}

		if (pos_1_y > pos_2_y)
		{
			pos_1_y = pos_1_y ^ pos_2_y;
			pos_2_y = pos_1_y ^ pos_2_y;
			pos_1_y = pos_1_y ^ pos_2_y;
		} //xor swap

		if (pos_1_x < 0)
		{
			pos_1_x = 0;
		}
		if (pos_1_y < 0)
		{
			pos_1_y = 0;
		}
		if (pos_2_x >= image_width)
		{
			pos_2_x = image_width - 1;
		}
		if (pos_2_y >= image_height)
		{
			pos_2_y = image_height - 1;
		}

		for (int i = pos_1_x; i <= pos_2_x; i++)
		{
			for (int j = pos_1_y; j <= pos_2_y; j++)
			{
				dptr_mask_area_editor_final[i][j] = 1;
			}
		}
	}

	void area_set_total_editor(bool Boolean) //마음에 드는 영역을 0이나 1로 바꿔줌
	{
		for (int i = 0; i < image_width; i++)
		{
			for (int j = 0; j < image_height; j++)
			{
				dptr_mask_area_editor_final[i][j] = Boolean;
			}
		}
	}

	void clear_editor() //mask 초기화. area_set_total_editor(0)랑 같은 역할.
	{
		for (int i = 0; i < image_width; i++)
		{
			for (int j = 0; j < image_height; j++)
			{
				dptr_mask_area_editor_final[i][j] = 0;
			}
		}
	}

	void test_draw_0()
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area(0, 0, 19, 19); // 영역선택 전체

		arg_editor

		(*layer_arr[0]).pixel(10, 10, 1, 0xff, 0xff, 0); //포토샵 처럼 #a000ff 로 나타 낼까??
		(*layer_arr[0]).pixel(10, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(10, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 12, 1, 0xed, 0x1c, 0x24);

		(*layer_arr[1]).pixel(8, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 12, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(8, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 11, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 10, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 9, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 9, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(9, 8, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 8, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 8, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(0, 7, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 7, 1, 0xA0, 0x00, 0xff); // 행렬 집합으로 처리해서 단숨에 처리하는 방법을 생각해 보아야 겠다.
		(*layer_arr[1]).pixel(14, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 7, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 7, 1,   0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(11, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 6, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 6, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 5, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(9, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 4, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(11, 3, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(12, 3, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 3, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height); //순서에 따라 레이어를 다르게 겹칠 수 있음.
		(*layer_arr[0]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void test_draw_1()
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area(0, 0, 19, 19); // 영역선택 전체

		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1); // 영역선택 전체

		(*layer_arr[0]).pixel(10, 10, 1, 0xff, 0xff, 0); //포토샵 처럼 #a000ff 로 나타 낼까??
		(*layer_arr[0]).pixel(10, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(10, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 12, 1, 0xed, 0x1c, 0x24);

		(*layer_arr[1]).pixel(8, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 12, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(8, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 11, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 10, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 9, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 9, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(9, 8, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 8, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 8, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(0, 7, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 7, 1, 0xA0, 0x00, 0xff); // 행렬 집합으로 처리해서 단숨에 처리하는 방법을 생각해 보아야 겠다.
		(*layer_arr[1]).pixel(14, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 7, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 7, 1,   0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(11, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 6, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 6, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 5, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(9, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 4, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(11, 3, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(12, 3, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 3, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[0]).store(edit_refined_data_copy_buff, image_width, image_height);
		(*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void test_draw_2()
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area(0, 0, 19, 19); // 영역선택 전체

		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1); // 영역선택 전체

		(*layer_arr[0]).pixel(10, 10, 1, 0xff, 0xff, 0); //포토샵 처럼 #a000ff 로 나타 낼까??
		(*layer_arr[0]).pixel(10, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(10, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 12, 1, 0xed, 0x1c, 0x24);

		(*layer_arr[1]).pixel(8, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 12, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(8, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 11, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 10, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 9, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 9, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(9, 8, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 8, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 8, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(0, 7, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 7, 1, 0xA0, 0x00, 0xff); // 행렬 집합으로 처리해서 단숨에 처리하는 방법을 생각해 보아야 겠다.
		(*layer_arr[1]).pixel(14, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 7, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 7, 1,   0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(11, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 6, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 6, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 5, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(9, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 4, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(11, 3, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(12, 3, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 3, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height);
		// (*layer_arr[0]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void test_draw_3()
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area(0, 0, 19, 19); // 영역선택 전체

		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1); // 영역선택 전체

		(*layer_arr[0]).pixel(10, 10, 1, 0xff, 0xff, 0); //포토샵 처럼 #a000ff 로 나타 낼까??
		(*layer_arr[0]).pixel(10, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(10, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(11, 12, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[0]).pixel(12, 12, 1, 0xed, 0x1c, 0x24);

		(*layer_arr[1]).pixel(8, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 12, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 12, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(8, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 11, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 11, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 10, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 10, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 10, 1, 0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(9, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 9, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 9, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 9, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(9, 8, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 8, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 8, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 8, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(0, 7, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 7, 1, 0xA0, 0x00, 0xff); // 행렬 집합으로 처리해서 단숨에 처리하는 방법을 생각해 보아야 겠다.
		(*layer_arr[1]).pixel(14, 7, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 7, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 7, 1,   0xA0, 0x00, 0xff);

		(*layer_arr[1]).pixel(11, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 6, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 6, 1, 0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(16, 6, 1,   0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(9, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(10, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 5, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 5, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(8, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(9, 5, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(11, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(12, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 4, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 4, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).pixel(10, 4, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(11, 3, 1,   0xA0, 0x00, 0xff);
		// (*layer_arr[1]).pixel(12, 3, 1,   0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(13, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(14, 3, 1, 0xA0, 0x00, 0xff);
		(*layer_arr[1]).pixel(15, 3, 1, 0xA0, 0x00, 0xff);

		// (*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height);
		(*layer_arr[0]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	/*
	선 그리기
	1. 점을 여러 번 입력받음(.pixel함수 사용)
	- 사용자가 원할 때마다 레이어 변경 가능
	2. 한 번에 
	3. 레이어 겹칠 순서 사용자가 지정

	pos_x1-pos_x가
	홀수인 경우, 중앙 픽셀이 중복됨_둘 다 1
	짝수인 경우, 문제

	선 1개 그리는 함수
	선 여러 번 그리는 것_선 1개 함수 여러 번 반복 호출
	*/

	void test_line(int pos_x, int pos_y, int pos_x1, int pos_y1, bool draw, uint8_t R, uint8_t G, uint8_t B)
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area_set_total(1); // 영역선택 전체

		int f, df1, df2, temp, delta_x, delta_y;
		float gradient;		//직선의 기울기
		if (pos_x > pos_x1) // pos_x1의 값이 pos_x보다 항상 크거나 같게
		{
			temp = pos_x1;
			pos_x1 = pos_x;
			pos_x = temp;

			temp = pos_y1;
			pos_y1 = pos_y;
			pos_y = temp;
		}
		delta_x = pos_x1 - pos_x;
		delta_y = pos_y1 - pos_y;
		gradient = (float)delta_y / float(delta_x);
		f = 2 * delta_y - delta_x; // 중단점
		df1 = 2 * delta_y;
		df2 = 2 * (delta_y - delta_x);

		int x = pos_x;
		int y = gradient * (x - pos_x) + pos_y;

		// 직선의 방정식: y = gradient(x-pos_x)+pos_y
		// y > gradient(x-pos_x)+pos_y면 해당 좌표는 직선보다 위에 있음.
		// y < gradient(x-pos_x)+pos_y면 해당 좌표는 직선보다 아래에 있음.
		// 판변식: -2*delta_x(y - pos_y) + 2*delta_y(y - pos_x)
		// 판별식이 < 0 :직선 위에 위치, 판별식 > 0 :직선 아래에 위치

		// 0 <= gradient <= 1
		for (int x = pos_x; x <= pos_x1; ++x)
		{
			(*layer_arr[0]).pixel(x, gradient * (x - pos_x) + pos_y, 1, R, G, B); //점 그리기
			if (f < 0)															  //중단점이 0보다 작으면, 알맞은 판별식으로 갱신. y값은 그대로
			{
				f += df1;
			}
			else //중단점이 0보다 크거나 같으면, 알맞은 판별식으로 갱신. y값 증가
			{
				++y;
				f += df2;
			}
		}

		(*layer_arr[0]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void test_multi_line()
	{
		/*
		int a = 0;
		char answer;
		while(a < 4)	//일단 0에서 3번 레이어까지만
		{
			cout << "레이어 " << a << "번 사용을 유지하시겠습니까?(y/n)"
			cin >> answer;
			if(answer == n || answer == N)
			{
				a++;
				(*layer_arr[a]).clear();
				(*layer_arr[a]).area_set_total(1);

			}

		}
		*/
	}

	void test_circle(int pos_x, int pos_y, int radius, bool draw, uint8_t R, uint8_t G, uint8_t B)
	{
		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1);

		int r2 = 0;
		for (int i = pos_x - radius; i < pos_x + radius + 1; i++)
		{
			for (int j = pos_x - radius; j < pos_x + radius + 1; j++)
			{
				r2 = (i - pos_x) * (i - pos_x) + (j - pos_y) * (j - pos_y);
				if (r2 > (radius - 1) * (radius - 1) && r2 <= (radius) * (radius))
				{
					(*layer_arr[1]).pixel(i, j, draw, R, G, B);
				}
			}
		}
		(*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void test_circle_filled(int pos_x, int pos_y, int radius, bool draw, uint8_t R, uint8_t G, uint8_t B)
	{
		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1);

		int r2 = 0;
		for (int i = pos_x - radius; i < pos_x + radius + 1; i++)
		{
			for (int j = pos_x - radius; j < pos_x + radius + 1; j++)
			{
				r2 = (i - pos_x) * (i - pos_x) + (j - pos_y) * (j - pos_y);
				if (r2 <= radius * radius)
				{
					(*layer_arr[1]).pixel(i, j, draw, R, G, B);
				}
			}
		}
		(*layer_arr[1]).store(edit_refined_data_copy_buff, image_width, image_height);
	}

	void open_file(const char *f_name, char I_O)
	{
		if (I_O == 'I' || I_O == 'i')
		{
			f_in_image.open(f_name, ios_base::binary);
		}
		if (I_O == 'O' || I_O == 'o')
		{
			f_out_image.open(f_name, ios_base::binary);
		}
	}

	// "sample_iw_4_4_4_size_bmp.bmp"

	int get_file_size()
	{
		cout << bitmap_FILE_HEADER.bfSize << endl;
		return bitmap_FILE_HEADER.bfSize;
	}

	int get_data_size()
	{
		// f_in_image.seekg(18, ios::end);

		// unsigned int temp_buff = 0;

		// int n = 0;

		// f_in_image.read((char*)&temp_buff, 1);

		return 0;
	}

	void read_header_bmp()
	{
		char *buff_ptr = NULL;
		buff_ptr = head_buff;
		f_in_image.seekg(0, ios::beg);
		f_in_image.read(buff_ptr, 54);

		// f_in_image.seekg(0, ios::beg);
		// f_in_image.read(head_buff, 54);

		cout << (void *)head_buff << endl;

		cout << endl;
		cout << endl;
		// memmove(edit_head_buff, buff_ptr, 54);//왜 에러 일까남... 디버그 돌리고 싶당...ㅠㅠ//편집하다보니 new 키워드 보다 함수의 실행 시점이 늦어진 것이 문제였음.
		cout << endl;
	}

	void read_data_bmp()
	{
		char *buff_ptr = NULL;
		//byte_for_pixel 한 픽셀을 차지하는 바이트 수 현재 RGB 3색 각 1바이트 씩 총 3바이트
		buff_ptr = rare_buff;
		f_in_image.seekg(54, ios::beg);
		f_in_image.read(buff_ptr + 54, bitmap_INFO_HEADER.biSizeImage);

		int height = image_height;
		int width = image_width;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount / 8);

		int length_of_row = width * byte_for_pixel / 4 + (width * byte_for_pixel % 4 != 0);
		int length_of_col = height;

		int i = 0;

		// f_in_image.seekg(54, ios::beg);
		// f_in_image.read(package_data_buff, bitmap_INFO_HEADER.biSizeImage);
		cout << "1" << endl;
		memmove(package_data_buff, buff_ptr + 54, bitmap_INFO_HEADER.biSizeImage); //디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??
		cout << "2" << endl;
		memset(reverse_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);
		cout << "3" << endl;
		cout << (void *)reverse_data_buff << endl;
		while (i < length_of_col)
		{
			cout << "i : " << i << endl;
			cout << (void *)reverse_data_buff + ((length_of_col - i - 1) * 4 * length_of_row) << endl;

			memmove(reverse_data_buff + ((length_of_col - i - 1) * 4 * length_of_row), package_data_buff + (i * 4 * length_of_row), 4 * length_of_row);
			i = i + 1;
		}
		cout << "4" << endl;

		i = 0;

		memset(edit_refined_data_buff, 0, pure_size_of_data);
		cout << endl;
		while (i < length_of_col)
		{
			memmove(edit_refined_data_buff + (i * byte_for_pixel * width), reverse_data_buff + (i * 4 * length_of_row), byte_for_pixel * width);

			i = i + 1;
		}

		// f_in_image.read((char*) &temp_buff, editor.image_width*editor.image_height*byte_for_pixel);
	}

	void edit_head_load() //헤더를 수동으로 읽어 구조체가 차지하는 메모리를 낭비하지 않는 방법도 나중에 시도해 보자
	{
		char *buff_ptr = NULL;
		buff_ptr = head_buff;

		memmove(edit_head_buff, buff_ptr, 54);

		buff_ptr = edit_head_buff;

		void *temp_F_header_struct = &bitmap_FILE_HEADER;
		void *temp_I_header_struct = &bitmap_INFO_HEADER;

		uint32_t bit_temp_1 = 0;
		uint32_t bit_temp_2 = 0;

		int n = 1;
		int m = 0;

		cout << (void *)&bitmap_FILE_HEADER << endl;
		cout << (void *)&(bitmap_FILE_HEADER.bfType) << endl;
		cout << (void *)&(bitmap_FILE_HEADER.bfSize) << endl;
		cout << (void *)&(bitmap_FILE_HEADER.bfReserved1) << endl;

		cout << temp_F_header_struct << endl;

		bit_temp_1 |= (int)(*buff_ptr);

		cout.fill('0');
		cout.width(8);
		cout << bit_temp_1 << endl;

		bit_temp_1 = bit_temp_1 << 8;
		buff_ptr = buff_ptr + 1;
		bit_temp_1 |= (int)(*buff_ptr);

		cout.fill('0');
		cout.width(8);
		cout << bit_temp_1 << endl;

		buff_ptr = buff_ptr + 1;

		(*(uint16_t *)temp_F_header_struct) = bit_temp_1;

		temp_F_header_struct = temp_F_header_struct + 4;
		buff_ptr = edit_head_buff + 1;
		while (n < 5)
		{
			bit_temp_1 = NULL;
			m = 0;
			buff_ptr = buff_ptr + file_head[n];
			while (m < file_head[n])
			{
				// (*temp_F_header_struct) = (*buff_ptr);
				// bit_temp_1 = bit_temp_1<<8;
				bit_temp_1 <<= 8;
				bit_temp_2 = (*buff_ptr);
				bit_temp_2 &= 0x000000ff;
				bit_temp_1 |= bit_temp_2;

				// (*(temp_F_header_struct)) |= 0xaa;

				cout << hex;
				cout.fill('0');
				cout.width(8);
				cout << (int)(*buff_ptr) << endl;
				cout << bit_temp_2 << endl;
				cout << bit_temp_1 << "/" << endl;

				// temp_F_header_struct = temp_F_header_struct + 1;
				buff_ptr = buff_ptr - 1;
				m = m + 1;
			}
			// temp_F_header_struct = temp_F_header_struct+file_head[n];
			(*(uint32_t *)temp_F_header_struct) = (uint32_t)bit_temp_1;
			temp_F_header_struct = temp_F_header_struct + 4;
			cout << (void *)temp_F_header_struct << endl;

			buff_ptr = buff_ptr + file_head[n];
			n = n + 1;
		}

		cout << endl;

		buff_ptr = edit_head_buff + 13;
		n = 0;
		while (n < 11)
		{
			bit_temp_1 = 0;
			m = 0;
			buff_ptr = buff_ptr + info_head[n];
			while (m < info_head[n])
			{
				// (*temp_I_header_struct) = (*buff_ptr);
				// (*(char*)temp_I_header_struct) |= 0xaa;
				bit_temp_1 <<= 8;
				bit_temp_2 = (*buff_ptr);
				bit_temp_2 &= 0x000000ff;
				bit_temp_1 |= bit_temp_2;

				// cout<<dec;
				cout << hex;
				cout.fill('0');
				cout.width(8);
				cout << (int)(*buff_ptr) << endl;
				cout << bit_temp_2 << endl;
				cout << bit_temp_1 << "/" << endl;

				// temp_I_header_struct = temp_I_header_struct + 1;
				buff_ptr = buff_ptr - 1;
				m = m + 1;
			}
			// temp_I_header_struct = temp_I_header_struct+info_head[n];
			(*(uint32_t *)temp_I_header_struct) = (uint32_t)bit_temp_1;

			cout << (*(uint32_t *)temp_I_header_struct) << endl;
			cout << (void *)temp_I_header_struct << endl;
			temp_I_header_struct = temp_I_header_struct + 4;
			buff_ptr = buff_ptr + info_head[n];
			n = n + 1;
		}
	}

	void edit_data_load()
	{
		memmove(edit_refined_data_copy_buff, edit_refined_data_buff, pure_size_of_data);
	}

	void edit_head_store()
	{
		memmove(head_buff, edit_head_buff, 54);
	}

	void edit_data_store()
	{
		int height = image_height;
		int width = image_width;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount / 8);

		int length_of_row = width * byte_for_pixel / 4 + (width * byte_for_pixel % 4 != 0);
		int length_of_col = height;

		int i = 0;
		// cout<<endl<<endl<<(void*)reverse_data_buff<<endl<<endl;
		// cout<<sizeof(reverse_data_buff)<<endl<<endl;
		// cout<<bitmap_INFO_HEADER.biSizeImage<<endl;
		memset(reverse_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);

		while (i < length_of_col)
		{
			memmove(reverse_data_buff + (i * 4 * length_of_row), edit_refined_data_buff + (i * byte_for_pixel * width), byte_for_pixel * width); //*********************************************** 이곳에 최종 마스크가 영향을 주도록 수정하여야 함.
			i = i + 1;
		}

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
				if (dptr_mask_area_editor_final[x][y])
				{
					memmove(reverse_data_buff + (byte_for_pixel * x) + (4 * length_of_row * (height - (y + 1))), edit_refined_data_copy_buff + (byte_for_pixel * x) + (byte_for_pixel * width * (height - (y + 1))), byte_for_pixel); //*********************************************** 이곳에 최종 마스크가 영향을 주도록 수정하여야 함.
				}
		}

		memset(package_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);

		i = 0; // 상수 초기화가 되지 않은 경우 복사가 되지 않아 화면이 이상해 지는 버그 발생
		while (i < length_of_col)
		{
			// memmove(package_data_buff+(i*4*length_of_row), reverse_data_buff+((length_of_col-i-1)*4*length_of_row), 4*length_of_row);
			memmove(package_data_buff + ((length_of_col - i - 1) * 4 * length_of_row), reverse_data_buff + (i * 4 * length_of_row), 4 * length_of_row);
			i = i + 1;
		}
	}

	void reconstitution_data_bmp()
	{
		char *buff_ptr = NULL;
		buff_ptr = rare_buff;

		memmove(buff_ptr, head_buff, 54); //디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??

		memmove(buff_ptr + 54, package_data_buff, bitmap_INFO_HEADER.biSizeImage); //디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??

		// f_in_image.read((char*) &temp_buff, editor.image_width*editor.image_height*byte_for_pixel);
	}

	void write_data_bmp(const char *f_out_name) //"sample_binary_OUTPUT_2.bmp"
	{
		open_file(f_out_name, 'O');
		f_out_image.seekp(0, ios::beg);
		f_out_image.write(rare_buff, bitmap_FILE_HEADER.bfSize);
		f_out_image.close();
	}

	void save_as(const char *f_out_name)
	{
		edit_head_store();
		edit_data_store();
		reconstitution_data_bmp();
		write_data_bmp(f_out_name);
	}

	void print_header_bmp()
	{
		f_in_image.seekg(0, ios::beg);

		int n = 0;
		int m = 0;
		unsigned int temp_buff = 0;

		while (n < 5)
		{
			m = 0;
			while (m < file_head[n])
			{
				f_in_image.read((char *)&temp_buff, 1);
				cout << hex;
				cout.fill('0');
				cout.width(2);
				cout << temp_buff << " ";
				m = m + 1;
			}

			cout << endl;
			n = n + 1;
		}

		cout << endl;

		n = 0;
		while (n < 11)
		{
			m = 0;
			while (m < info_head[n])
			{
				f_in_image.read((char *)&temp_buff, 1);
				cout << hex;
				cout.fill('0');
				cout.width(2);
				cout << temp_buff << " ";
				m = m + 1;
			}

			cout << endl;
			n = n + 1;
		}
	}

	void print_data_bmp() //data가 어떻게 되어있는지 출력
	{
		f_in_image.seekg(54, ios::beg);
		unsigned int temp_buff = 0;
		int height = image_height;
		int width = image_width;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount / 8);

		int length_of_row = width * byte_for_pixel / 4 + (width * byte_for_pixel % 4 != 0);
		int length_of_col = height;

		cout << length_of_row << endl;
		cout << length_of_col << endl;

		int i = 0;
		int j = 0;
		int k = 0;
		i = 0;
		while (i < length_of_col)
		{
			cout << endl;

			j = 0;
			while (j < length_of_row)
			{
				k = 0;
				while (k < 4) //패키지 단위 4바이트씩 읽기
				{
					f_in_image.read((char *)&temp_buff, 1);
					cout.fill('0');
					cout.width(2);
					cout << hex;
					cout << (int)temp_buff << " ";
					k = k + 1;
				}
				j = j + 1;
			}

			i = i + 1;
		}
		f_in_image.read((char *)&temp_buff, 1);
		cout.fill('0');
		cout.width(2);
		cout << hex;
		cout << (int)temp_buff << " ";

		f_in_image.read((char *)&temp_buff, 1);
		cout.fill('0');
		cout.width(2);
		cout << hex;
		cout << (int)temp_buff << " ";
	}

	~bmp_editor() //소멸자.
	{
		f_in_image.close(); //파일 스트림 사용한 거 종료.
		delete rare_buff;

		delete head_buff;
		delete edit_head_buff;

		delete package_data_buff;
		delete reverse_data_buff;	   //munmap_chunk(): invalid pointer
		delete edit_refined_data_buff; //free(): invalid pointer
		delete edit_refined_data_copy_buff;

		for (int i = 0; i < image_height; i++)
		{
			delete dptr_mask_area_editor_final[i];
		}
		delete dptr_mask_area_editor_final;

		bool **dptr_mask_area_editor_final = nullptr;
	}
};

class viewer
{

	SDL_Window *first_window = nullptr;
	SDL_Surface *window_surface = nullptr;
	SDL_Surface *image_surface = nullptr; //이미지를 저장해서 window_surface로 전달

	bool isRunning = 0;
	SDL_Event ev;

public:
	viewer(const char *name_of_window, int width, int height)
	{
		isRunning = 1;

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			cout << "SDL Video Initialize failed" << endl
				 << SDL_GetError() << endl;
		}
		else
		{
			first_window = SDL_CreateWindow(name_of_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		}
		if (first_window == nullptr)
		{
			cout << "window Creation failed" << SDL_GetError() << endl;
		}
		else
		{
			window_surface = SDL_GetWindowSurface(first_window); //생성 오류 애러 시 다른 함수들에서 에러 처리 필요할 가능성 있음.
		}
	}

	void create_d24_Image_surface_in_SDL(int width, int height)
	{
		if (first_window == nullptr)
		{
			cout << "window Creation failed" << SDL_GetError() << endl;
		}
		else
		{
			if (image_surface == nullptr)
			{
				image_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);
				memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel) * width * height); //색상을 런 타이밍 중에 변경하는 것 성공!!
			}
			else
			{
				SDL_FreeSurface(image_surface);
				image_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);
				memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel) * width * height); //색상을 런 타이밍 중에 변경하는 것 성공!!
			}
			if (image_surface == nullptr)
			{
				cout << "Image Load Failed" << endl
					 << SDL_GetError() << endl;
			}
		}
	}

	void image_deilvery(char *refined_data_deilvery_buff)
	{
		if (image_surface == nullptr)
		{
			cout << "Image Load Failed" << endl
				 << SDL_GetError() << endl;
		}
		else
		{
			memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel) * ((*image_surface).w) * ((*image_surface).h));
			memmove((*image_surface).pixels, refined_data_deilvery_buff, ((*image_surface).format->BytesPerPixel) * ((*image_surface).w) * ((*image_surface).h));
		}
	}

	void image_safty_load(int width, int height, char *refined_data_deilvery_buff)
	{
		create_d24_Image_surface_in_SDL(width, height);
		image_deilvery(refined_data_deilvery_buff);
	}

	void screen_float()
	{
		if (first_window == nullptr)
		{
			cout << "window Creation failed" << SDL_GetError() << endl;
		}
		else
		{
			if (image_surface == nullptr)
			{
				cout << "Image Load Failed" << endl
					 << SDL_GetError() << endl;
			}
			else
			{
				//Important function that show Image in program
				SDL_BlitSurface(image_surface, nullptr, window_surface, nullptr);
				SDL_UpdateWindowSurface(first_window);

				// cout<<sizeof((*image_surface).pixels)<<endl;
				// cout<<(*image_surface).w<<endl;
				// cout<<(*image_surface).h<<endl;
				// cout<<(int)((*image_surface).format->BytesPerPixel)<<endl;
				// cout<<(int)((*image_surface).format->BitsPerPixel)<<endl;
				// cout<<(int)((*image_surface).format->Rmask)<<endl;
				// cout<<(int)((*image_surface).format->Gmask)<<endl;
				// cout<<(int)((*image_surface).format->Bmask)<<endl;
				// cout<<(int)((*image_surface).format->Amask)<<endl;
				// cout<<(int)((*image_surface).format->Rloss)<<endl;
				// cout<<(int)((*image_surface).format->Gloss)<<endl;
				// cout<<(int)((*image_surface).format->Bloss)<<endl;
				// cout<<(int)((*image_surface).format->Aloss)<<endl;
			}
		}
	}

	int event_activator() //이벤트나 인터럽트를 사용하는 방법을 알아봐야 겠다.
	{
		int LOOP_state = 1;

		while (SDL_PollEvent(&ev) != 0) //have event been occurred?
		{
			if (ev.type == SDL_QUIT)
			{
				LOOP_state = 0;
				SDL_FreeSurface(image_surface);
				SDL_DestroyWindow(first_window);
				SDL_Quit(); //SDL_Quit(); is included
			}
		}
		return LOOP_state;
	}

	//First window is allocated by dynamic mathod by, SDL_CreateWindow() function, therefore it have to be free after use
	~viewer()
	{
		isRunning = 0;
		// SDL_FreeSurface(image_surface);
		cout << "9" << endl;

		// SDL_DestroyWindow(first_window);
		cout << "10" << endl;
		image_surface = nullptr;
		window_surface = nullptr;

		first_window = nullptr;

		// SDL_Quit();
	}
};

void draw_robot_sim(bmp_editor* arg_editor, robot* arg_robot_sim)
{

	double position_P[5][3] = {0, };

	(*((*arg_editor).layer_arr[1])).clear();
	(*((*arg_editor).layer_arr[1])).area_set_total(1);

	position_P = (*arg_robot_sim).position;

	(*((*arg_editor).layer_arr[1])).pixel

	(*((*arg_editor).layer_arr[1])).store;


}

int main()
{
	/*
	콜백함수 수준까지는 구현하지않아 창을 열고 닫을 때 항상 조건문의 조건을 통해 수동적으로 감시할 필요가 있다.
	*/
	int Delay_time = 50; //ms 단위
	viewer screen_0("strawberry", 1000, 1000);
	bmp_editor editor("_execute_test_area_iw/sample_iw_19_1000_1000_size_bmp.bmp"); //파일의 경로를 현재 디렉토리로 부터 상세히 작성하시오.

	cout << "::" << editor.bitmap_INFO_HEADER.biHeight << endl;
	cout << "::" << editor.bitmap_INFO_HEADER.biWidth << endl;
	cout << "::" << editor.bitmap_INFO_HEADER.biSize << endl;
	cout << "::" << editor.bitmap_INFO_HEADER.biSizeImage << endl;

	editor.area_set_total_editor(1); //영역 선택(전체)

	editor.edit_data_load();
	editor.test_line(100, 100, 900, 900, 1, 0xa0, 0x00, 0xff);
	screen_0.image_safty_load(1000, 1000, editor.edit_refined_data_copy_buff);
	screen_0.screen_float();
	editor.save_as("_execute_test_area_iw/sample_line_OUTPUT_0.bmp");

	robot_sim.motor_combo_calibration(&robot_arm_1, &func_gripper, 36, 0, 10, M_PI_2, 255, 5); //last argument is number_of_motors
	robot_sim.combine_ordering(&robot_arm_1, &func_gripper, 40, 10, 15, M_PI_2, 255, 100);

	while (screen_0.event_activator() != 0)
	{
		//화면이 켜져 있는 동안 할 작업// 매루프마다 이벤트가 확인됨.
		//바르게 반복되는 작업을 실행하기에 적합. 반복주기가 긴 코드는 응답 시간 지연을 일으킴으로 부적합.
		/*예시
		// editor.edit_data_load();
		// editor.test_line(100, 100, 900, 900, 1, 0xa0, 0x00, 0xff);
		// screen_0.image_safty_load(1000, 1000, editor.edit_refined_data_copy_buff);
		// screen_0.screen_float();
		*/

		robot_sim.virtual_driving();
		editor.edit_data_load();


		SDL_Delay(Delay_time); //20fps 설정
	}


	combine_ordering(&robot_arm_1, &func_gripper, 36, 0, 10, M_PI_2, 255, 100);

	virtual_driving();

}
