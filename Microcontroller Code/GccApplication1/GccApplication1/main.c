/*
 * Analog input.c
 *
 * Created: 7/18/2022 11:10:42 AM
 * Author : Anup
 */ 

#ifndef F_CPU
#define F_CPU 1000000 
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main(void)
{
	int delay_ms = 1000;
	
	DDRD = 0xFF;
	DDRC = 0xFF;
	
	
	DDRB = 0xFF;
	// DDRA te direction thik kore deya lagbe na, when we are working with analog signal
	ADMUX	=	0b01100110; // AVCC = 5 V selected
	ADCSRA	=	0b10000011; // prescalar = 8
	
	_delay_ms(5000);
	Lcd4_Init();
	_delay_ms(5000);
	while(1)
	{
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("Voltage: ");
		
		// take input
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC))
		{
			// after conversion is done, ADSC bit will be 0, so, until then, we wait and do nothing
		} // conversion finished (ADC e converted data thakbe)
		
		// convert to voltage
		int adcl = ADCL;
		adcl = adcl >> 6;
		int adch = ADCH;
		adch = adch << 2;
		int adc = adch | adcl;
		
		float volt = adc * (5.0/1024);
		//volt = 1.22;
		char result[8];
		dtostrf(volt, 1, 2, result);	// double-to-string-format 1 precision, 2 width
		
		if(volt >=2) {
			PORTB =	1;
		} else {
			PORTB = 0;
		}

		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(result);

	}
}