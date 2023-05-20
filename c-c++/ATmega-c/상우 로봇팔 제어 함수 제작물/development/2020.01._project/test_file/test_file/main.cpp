/*
 * test_file.cpp
 *
 * Created: 2020-02-14 오후 9:07:18
 * Author : kimzt
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b00111111;
    /* Replace with your application code */
    while (1) 
    {
			PORTD = 0b100100;
			_delay_ms(1000);
			PORTD = 0b100001;
			_delay_ms(1000);
			PORTD = 0b001001;
			_delay_ms(1000);
			PORTD = 0b011000;
			_delay_ms(1000);
			PORTD = 0b010010;
			_delay_ms(1000);
			PORTD = 0b000110;
			_delay_ms(1000);
    }
}

