#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
void led_init()
{
    DDRE |= (1<<PE2);
    PORTE|= (1<<PE2);
    DDRF = 0XFF;
	PORTF= 0xaa;
}

