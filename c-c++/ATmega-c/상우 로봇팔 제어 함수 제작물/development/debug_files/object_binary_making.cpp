#include<stdio.h>


int object_binary_making(int object_pin, int binary_int)
{
	int result_binary = 0;
	int not_binary = ~object_pin;
	result_binary |= (object_pin & binary_int);
	result_binary &= (not_binary | binary_int);
	return result_binary;
}


int main()
{
	printf("%d", object_binary_making(0b00111111, 0b10000101));
}
