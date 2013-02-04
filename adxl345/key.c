#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL
#include<util/delay.h>
void key_init()
{
	//set interrupt 4
	DDRE &=~(1<<PE4);
	PORTE|= (1<<PE4);

	EICRB = 0x00;

	EICRB|= (1<<ISC41)|(1<<ISC40);
	EIMSK = 0x00;
	EIMSK|= (1<<4);			//Enable interrupt
}
ISR(INT4_vect)
{
	cli();
//	_delay_ms(100);
	static char i=0;
	EIFR|= (1<<4);			//Write 1 to clear
	i++;
	if(i==2)
	{
		PORTF = 0X01;
		i=0;
	}
	else	
		PORTF = 0x02;
	sei();
}

