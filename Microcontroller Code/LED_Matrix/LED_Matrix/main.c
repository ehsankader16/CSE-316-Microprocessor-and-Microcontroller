/*
 * LED_Matrix.c
 *
 * Created: 6/19/2022 5:35:14 PM
 * Author : ASUS
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

int main(void)
{
		unsigned char in;
		DDRA = 0x00;
		DDRB = 0xFF;
		DDRC = 0xFF;
		while(1)
		{
				in = PINA;
				PORTB = (0x00) | (in>>4);

				PORTC = (0x00) | (in<<4);
		}
}