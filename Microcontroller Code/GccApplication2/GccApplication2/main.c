/*
 * GccApplication2.cpp
 *
 * Created: 8/25/2022 3:44:20 PM
 * Author : HP
 */

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0b00000000;	// 0-2 for reg1Addr & 2-5 for reg2Addr, 6 for regwrite, 7 for clk
	DDRB = 0b00000000;	// 0-3 for writeData & 4-7 for writeRegAddr
	DDRD = 0b11111000; // 0-2 for taking input for check register, 3-6 to output the value of the check register; we are doing this for testing purpose
	DDRC = 0b11111111;	// 0-3 for reg1Data & 4-7 for reg2Data

	unsigned char regArr[] = {0, 0, 0, 0, 0, 0};
	unsigned char reg1Addr, reg2Addr; //register address for reading
	unsigned char writeData=0, writeRegAddr=0; //data and address for writing
	unsigned char regWrite = 0;
	unsigned char prevClk = 0;
	unsigned char currClk = 0;

	unsigned checkRegAddr = 0; //


    while (1)
    {
		prevClk = currClk;
		currClk = (PINA & 0b10000000); //7 for clk
		if(regWrite && (prevClk && !currClk) && writeRegAddr) {
			regArr[writeRegAddr] = writeData;
		}
		reg1Addr = PINA & 0b0000111;
		reg2Addr = (PINA & 0b00111000) >> 3;

		writeData = PINB & 0b00001111;
		writeRegAddr = (PINB & 0b11110000) >> 4;

		regWrite = (PINA & 0b01000000); //6th bir for regwrite
		
		PORTC = regArr[reg1Addr] | (regArr[reg2Addr] << 4);

		checkRegAddr = (PIND & 0b00000111);
		PORTD = (regArr[checkRegAddr] << 3);

    }
}

