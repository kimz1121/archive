#include<stdio.h>
#include<conio.h>

int bit_new = -1;//countion 
int bit_old = -1;


int sicuential_bit_input(int bit_input, int* bit_new_s_adress, int* bit_old_s_adress)//���� ���縦 ���ؼ� �ּҸ� ����//it takes adress for deep copy
{//�ٲ��� ������쿡�� �Լ��� ����Ǿ� old�� new�� ���� ���� ���� �Ǿ� ������ ��Ȳ�� �����ϴ� ��� �߰�
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
