#include<avr/io.h>
#include"uart.h"
#define F_CPU 16000000UL
#include<util/delay.h>
#include<inttypes.h>
uint8_t HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
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
void PrintHex(uint8_t c)
{
	uart_putchar('0');
	uart_putchar('x');
	uart_putchar(HexTable[c>>4]);
	uart_putchar(HexTable[c&0x0f]);
}
int main()
{
	uart_init();
	uint8_t w=0;
	while(1)
	{
		for(w=0;w<256;w++)
		{
			PrintHex(w);
			_delay_ms(100);
			uart_putchar('\n');
			uart_putchar('\r');
		}
	}
	return 0;
}
