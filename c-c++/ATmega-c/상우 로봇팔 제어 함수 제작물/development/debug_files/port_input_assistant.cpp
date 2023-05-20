#include<stdio.h>

#define PINA 0;
#define PINB 0b00100111;
#define PINC 0;
#define PIND 0b10000011;
#define PINE 0;
#define PINF 0;

int port_input_assistant(char port_name, int object_pin)
{
	int temp = 0;
	int result = 0;
	switch(port_name)
	{
		case 'a':
		case 'A':
		temp = PINA;
		break;
		case 'b':
		case 'B':
		temp = PINB;
		break;
		case 'c':
		case 'C':
		temp = PINC;
		break;
		case 'd':
		case 'D':
		temp = PIND;
		break;
		case 'e':
		case 'E':
		temp = PINE;
		break;
		case 'f':
		case 'F':
		temp = PINF;
		break;
	}
	int i = 0;
	int n = 0;
	int array[8] = {0,};
	for(i = 0; i < 8; i = i + 1)
	{
		if(((object_pin>>i)%0b10))
		{
			array[n] = i;
			n = n + 1;
		}
	}
	for(i = 0; i < n; i = i + 1)
	{
		result |= ((temp>>array[i])%0b10)<<i;
	}
	return result;
}

int main()
{
	printf("%d\n", port_input_assistant('b', 0b00000011));
	printf("%d\n", port_input_assistant('b', 0b00100011));
	printf("%d\n", port_input_assistant('b', 0b11111111));
	printf("%d\n", port_input_assistant('d', 0b00000111));
	printf("%d\n", port_input_assistant('d', 0b00000111));
	printf("%d\n", port_input_assistant('d', 0b10000011));
}
