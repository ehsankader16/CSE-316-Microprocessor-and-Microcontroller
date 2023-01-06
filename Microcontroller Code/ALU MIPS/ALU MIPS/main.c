/*
 * GccApplication1.cpp
 *
 * Created: 8/2/2022 12:09:00 PM
 * Author : iRounge12
 */ 

#include <avr/io.h>

#define ADD 0
#define SUB 1
#define AND 3
#define OR 2
#define NOR 4
#define SLL 5
#define SRL 6

int main(void)
{
	DDRD = 0x00; // setting port D as input of OPCODE
	DDRA = 0xFF; // setting port A as input of registers (4LSB = reg1, 4MSB = reg2)
				 // in case of shift, 4MSB = shift amount
	DDRB = 0x00; // output of ALU (least 4 significant bits are result)
	DDRC = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
		char opcode = PIND & (0b00000111);
		char reg1 = PINB & 0b00001111;
		char reg2 = PINB & 0b11110000;
		reg2 = reg2 >> 4;
		char result = 0;
		if (opcode == ADD)
		{
			result = reg1 + reg2;
		}
		
		else if (opcode == SUB)
		{
			result = reg1 - reg2;
		}
		
		else if (opcode == AND)
		{
			result = reg1 & reg2;
		}
		
		else if (opcode == OR)
		{
			result = reg1 | reg2;
		}
		
		else if (opcode == NOR)
		{
			result = ~(reg1 | reg2);
		}
		
		else if (opcode == SLL)
		{
			result = reg1 << reg2;
		}
		
		else if (opcode == SRL)
		{
			result = reg1 >> reg2;
		}
		PORTA = result;
		PORTC = !result;
    }
}

