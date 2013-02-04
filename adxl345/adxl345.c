#include"i2c.h"
#include"uart.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include<util/delay.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#define Default_Gain GAIN_Two_Five
float Buf[3];
void adxl345_init()
{
	uint8_t buf[7];
	buf[0]=0x0B;
	buf[1]=0x08;
	buf[2]=0x08;
	buf[3]=0x80;
	buf[4]=0x00;
	buf[5]=0x00;
	buf[6]=0x05;
	write_bytes(0x31,1,&buf[0]);//-16~+16g,full-resolution
	write_bytes(0x2c,1,&buf[1]);//rate=25hz bandwith=12.5hz
	write_bytes(0x2d,1,&buf[2]);//power mode,measure enable
	write_bytes(0x2e,1,&buf[3]);//DATA_READY enable
	write_bytes(0x1e,1,&buf[4]);//xoff 
	write_bytes(0x1f,1,&buf[5]);//yoff
	write_bytes(0x20,1,&buf[6]);//zoff
}
void Accelerometer_Detect() 
{
	int x,y,z;
	uint8_t buf[6];
	read_bytes(0x32,6,buf);
	x=(buf[1]<<8)+buf[0];
	y=(buf[3]<<8)+buf[2];
	z=(buf[5]<<8)+buf[4];
	Buf[0]=(float)x*3.9;
	Buf[1]=(float)y*3.9;
	Buf[2]=(float)z*3.9;
	
}
int main()
{
	uint16_t a;
	int rv;
	uint8_t x;
		
	cli();
	uart_init();	
	i2c_init();
	/*for compass*/
	adxl345_init();
	led_init();
	key_init();
	Prints("adxl345_init ok!\n\r");
	sei();
	while(1)
	{
		Accelerometer_Detect();
		if(Buf[0]<0)
			Prints("Accelerometer_X<0!");
		Prints("Accelerometer_X=");
		PrintInt(Buf[0]);
		_delay_ms(300);
		uart_putchar('\n');
		uart_putchar('\r');
		if(Buf[1]<0)
			Prints("Accelerometer_Y<0!");
		Prints("Accelerometer_Y=");
		PrintInt(Buf[1]);
		_delay_ms(300);
		uart_putchar('\n');
		uart_putchar('\r');
		if(Buf[2]<0)
			Prints("Accelerometer_Z<0!");
		Prints("Accelerometer_Z=");
		PrintInt(Buf[2]);
		_delay_ms(300);
		uart_putchar('\n');
		uart_putchar('\r');
		uart_putchar('\n');
		uart_putchar('\r');
	}	
	return 0;
}
