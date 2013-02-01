#include"i2c.h"
#include"uart.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include<util/delay.h>
#include<inttypes.h>
enum{
	GAIN_One_Third = 0x20,
	GAIN_One_Nine  = 0x40,
	GAIN_Two_Five  = 0x60,
	GAIN_Four      = 0x80,
	GAIN_Four_Seven= 0xa0,
	GAIN_Five_Six  = 0xc0,
	GAIN_Eight_One = 0xe0,
};
#define Default_Gain GAIN_Two_Five
uint8_t Buf[6];
void hmc5883_init()
{
	uint8_t buf;
	buf=0x70;
	write_bytes(0x00,1,&buf);
	buf=Default_Gain;
	write_bytes(0x01,1,&buf);
	buf=0x00;
	write_bytes(0x02,1,&buf);
}
double CompassAngle(int Xoff,int Yoff,int Zoff)
{
	int x,y,z;
	uint8_t buf[6];
	double angle;
	read_bytes(0x03,6,buf);
	x=(buf[0]<<8)+buf[1];
	z=(buf[2]<<8)+buf[3];
	y=(buf[4]<<8)+buf[5];
	angle = atan2((double)y,(double)x)*(180/3.1415926)+180;
	angle*=10;
	return angle;
	
}
int main()
{
	uint16_t a;
	int rv;
	uint8_t x;
		
	uart_init();	
	i2c_init();
	/*for compass*/
	uint8_t buf;
	double angle=0;
	uint16_t angle1=0;
	hmc5883_init();
	Prints("hmc5883_init ok!\n\r");
	while(1)
	{
		buf = 0x00;
		write_bytes(0x02,1,&buf);
		
		angle=CompassAngle(0,0,0);
		angle1=angle;
		Prints("Azimuth=");
		PrintInt(angle1/10);
		uart_putchar('.');
		PrintUchar(angle1%10);
		_delay_ms(300);
	}	
	return 0;
}
