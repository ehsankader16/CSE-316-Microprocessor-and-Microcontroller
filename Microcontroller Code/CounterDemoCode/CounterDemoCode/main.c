/*
 * CounterDemoCode.c
 *
 * Created: 6/18/2022 6:09:24 PM
 * Author : ASUS
 */ 

#include <avr/io.h>
#define F_CPU 1000000 // Clock Frequency
#include <util/delay.h>



int main(void)
{

	DDRA = 0b00000000;
	DDRD = 0b00001111;
	unsigned char count = 0;

	while(1)
	{

		unsigned char in = PINA;
		if (in & 0b00000001) {
			count++;
			if(count == 16)
				count = 0;
			PORTD = count;
			_delay_ms(1000);
		}

	}

}

