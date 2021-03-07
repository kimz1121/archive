#include<stdio.h>
#include<conio.h>

int bit_new = -1;//countion 
int bit_old = -1;


int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress)//깊은 복사를 위해서 주소를 받음//it takes adress for deep copy
{//바뀌지 않은경우에도 함수가 실행되어 old와 new가 서로 같은 값이 되어 버리는 상황을 방지하는 기능 추가
	if(*bit_new_s_adress == -1)
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
}
int main()
{
	int a = 0;
	while(1)
	{
		scanf("%d", &a);
		sicuential_bit_input(a, &bit_new, &bit_old);
		printf("%d   %d\n", bit_new, bit_old);
	}
	
	
}
