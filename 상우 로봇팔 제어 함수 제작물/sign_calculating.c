#include<stdio.h>

//<시작>========================================
//thisfunction calculate hall sensor signals and than return output signals by situation.

void bin(int decimal)
{
    int binary[20] = { 0, };

    int position = 0;
    while (1)
    {
        binary[position] = decimal % 2;    // 2로 나누었을 때 나머지를 배열에 저장
        decimal = decimal / 2;             // 2로 나눈 몫을 저장

        position++;    // 자릿수 변경

        if (decimal == 0)    // 몫이 0이 되면 반복을 끝냄
            break;
    }
	int i;
    // 배열의 요소를 역순으로 출력
    for(i = position - 1; i >= 0; i--)
    {
        printf("%d", binary[i]);
    }

    printf("\n");
}













int sign_calculating(int hall_sensor_value, char diretion_value)
{
	int value = -1;
	int value_shifted = -1;
	char tranceformed_diretion_value = 'e';
	//direction_value can only have 3 status that is 'forward', 'backward', 'stop', 'natural'
/*
	int value_shifter(int value, int shift_distance)
	{
		value+shift_distance;
	}

*/	

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
	value_shifted = value + tranceformed_diretion_value;
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
//<끝>========================================

//QUSTNDPeKFK EKFFKWLSMS DUSTKS QKDTLR?
//쉬프트와 같은 방식으로 변화 
//정지상태에서의 값 을


int main()
{
	printf("%d\n", sign_calculating(0b110, 0));
	printf("%d\n", sign_calculating(0b110, 1));
	printf("%d\n", sign_calculating(0b110, 2));
	printf("%d\n", sign_calculating(0b110, 3));
	
	bin(sign_calculating(0b110, 0));//n
	bin(sign_calculating(0b110, 1));//f
	bin(sign_calculating(0b110, 2));//b
	bin(sign_calculating(0b110, 3));//s
	
}
