/*
 * MIPS.c
 *
 * Created: 8/24/2022 9:22:02 PM
 * Author : Pantha
 */ 

// Instruction memory

#include <avr/io.h>

char in = 0, out;
int instructions[256];

int main(void)
{
	DDRB = 0x00; // setting PORTB as input
	DDRD = 0xFF; 
	DDRA = 0xFF; // PORTA AND PORTD are instruction outputs
	DDRC = 0xFF; // PORTC has PC+1
	instructions[0] = 0b0000111110101111;
	instructions[1] = 0b1111000010100001;
	
    /* Replace with your application code */
    while (1) 
    {
		in = PINB;
		out = in+1;
		PORTD = out;
		char lower = (char) instructions[(int) in];
		char upper = (char) (instructions[(int) in] >> 8);
		PORTA = lower;
		PORTC = upper;
    }
}

