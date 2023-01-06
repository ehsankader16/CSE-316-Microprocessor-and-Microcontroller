/*
 * MIPS.c
 *
 * Created: 8/24/2022 9:22:02 PM
 * Author : Pantha
 */ 

// Control Unit

#include <avr/io.h>

char in = 0, out;
int controls[16];

int main(void)
{
	DDRB = 0x00; // setting PORTB as input
	DDRD = 0xFF; 
	DDRA = 0xFF; // PORTA AND PORTD are instruction outputs
	DDRC = 0xFF; // PORTC has PC+1
	controls[0] = 0b011000000001;
	controls[1] = 0b001000001010;
	controls[2] = 0b000000010001;
	controls[3] = 0b001000001000;
	controls[4] = 0b000000100001;
	controls[5] = 0b011000000010;
	controls[6] = 0b000010001000;
	controls[7] = 0b001000001110;
	controls[8] = 0b011000000011;
	controls[9] = 0b001000001011;
	controls[10] = 0b001101001000;
	controls[11] = 0b011000000000;
	controls[12] = 0b001000001101;
	controls[13] = 0b001000001001;
	controls[14] = 0b100000000000;
	controls[15] = 0b011000000100;
	

    /* Replace with your application code */
    while (1) 
    {
		in = 0x0F & PINB;
		char lower = (char) controls[(int) in];
		char upper = (char) (controls[(int) in] >> 8);
		PORTA = lower;
		PORTD = upper;
    }
}

