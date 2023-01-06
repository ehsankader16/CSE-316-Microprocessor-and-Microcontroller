/*
 * DatatMemory MIPS.c
 *
 * Created: 8/26/2022 5:42:36 PM
 * Author : ASUS
 */ 

#include <avr/io.h>

char data[32];

int main(void)
{
	DDRA = 0x00;	// address [A7-A4] data [A3-A0]
	DDRB = 0b11111000;	// clk [B0] memwrite [B1] memread [B2]	read data from mem [B7-B4]
	
	unsigned char memRead = 0;
	unsigned char memWrite = 0;
	unsigned char address = 0;
	unsigned char recentClk = 0;
	char incomingData = 0;
	int activated = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		memWrite = PINB;
		memRead = PINB;
		
		if (recentClk == 0 && (PINB & 0b00000001)) {
			activated = 1;
			recentClk = PINB & 0b00000001;
		} else if ((recentClk == 1) && ((PINB & 0b00000001) == 0)) {
			activated = 0;
			recentClk = PINB & 0b00000001;
		}
		
		
		
		address = PINA >> 4;
		incomingData = PINA & 0x0F;
		if ((incomingData & 0b00001000) && activated){
			incomingData |= 0b11110000;
		}
		
		if ((memWrite & 0b00000010) && activated) {
			data[address] = incomingData;
			
		}
		
		else if ((memRead & 0b00000100) && activated) { 
			PORTB = data[address] << 4;
			
		}
		
		// activated = 0;
		
		
		
    }
}

