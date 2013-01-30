#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
int main()
{
    DDRE |= 0XFF;
    PORTE|= 0XFF;
    DDRF = 0XFF;
    while(1)
	{
		PORTF = 0XAA;
		_delay_ms(1000);
		PORTF = 0X55;
		_delay_ms(1000);
	}
	return 0;
}

