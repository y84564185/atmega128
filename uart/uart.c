#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
void uart_init(void)
{
	UCSR1B = 0X00;
	UCSR1A = 0X00;
	UCSR1B&=~(1<<UCSZ12);
	UCSR1C|=(1<<UCSZ11)|(1<<UCSZ10);
#define BAUD	9600
#include<util/setbaud.h>
	UBRR1H	= UBRRH_VALUE;
	UBRR1L	= UBRRL_VALUE;
#if USE_2X
	UCSR1A |= _BV(U2X1);
#else
	UCSR1A &= ~(_BV(U2X1));
#endif
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
}
void uart_putchar(char c)
{
	loop_until_bit_is_set(UCSR1A,UDRE1);//wait until data register empty
	UDR1 = c;
}
char uart_getchar(void)
{
	loop_until_bit_is_set(UCSR1A,RXC1);	//wait until data exists
	return	UDR1;
}

int main()
{
	uart_init();
	while(1)
	{
		uart_putchar('H');
		_delay_ms(100);
		uart_putchar('e');
		_delay_ms(100);
		uart_putchar('l');
		_delay_ms(100);
		uart_putchar('l');
		_delay_ms(100);
		uart_putchar('o');
		_delay_ms(100);
		uart_putchar('\r');
		_delay_ms(100);
	}
	return 0;
}
