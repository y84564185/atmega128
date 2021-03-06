#include"i2c.h"
#include"uart.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include<util/delay.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include<stdlib.h>
#include<math.h>
#define pos 0
#define MAX_TILT 70
#define Default_Threadshold 50
struct acceleration{
	int x;
	int y;
	int z;
};
struct adxl345_data{
	struct acceleration last;	/*last measured data*/
	uint8_t threadshold;
	int update;
};
struct adxl345_data *adxl345;
static int  adxl345_position_map[][3][3]={
	{{0,-1,0},{1,0,0},{0,0,1}},/*top/upper-left*/
	{{1,0,0},{0,1,0},{0,0,1}},/*top/upper-right*/
	{{0,1,0},{-1,0,0},{0,0,1}},/*top/lower-right*/
	{{-1,0,0},{0,-1,0},{0,0,1}},/*top/lower-left*/
	{{0,1,0},{1,0,0},{0,0,-1}},/*bottom/upper-right*/
	{{-1,0,0},{0,1,0},{0,0,-1}},/*bottom/upper-left*/
	{{0,-1,0},{-1,0,0},{0,0,-1}},/*bottom/lower-left*/
	{{1,0,0},{0,-1,0},{0,0,-1}},/*bottom/lower-right*/
};
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
static int adxl345_data_filter(struct acceleration *accel,int data[])
{
	int threadshold = adxl345->threadshold;
	int update = 0;
	if((abs(adxl345->last.x - data[0]) > threadshold)||
	   (abs(adxl345->last.y - data[1]) > threadshold)||
	   (abs(adxl345->last.z - data[2]) > threadshold)){
		accel->x = data[0];
		accel->y = data[1];
		accel->z = data[2];
		update = 1;
	}else{
		*accel = adxl345->last;
	}
	adxl345->update = update;
	return 0;
}
void adxl345_measure(struct acceleration *accel)
{
	int i,j;
	int data[3],raw[3];
	float g;
	uint8_t buf[6];
	read_bytes(0x32,6,buf);
	raw[0]=(buf[1]<<8)+buf[0];//physical x
	raw[1]=(buf[3]<<8)+buf[2];//physical y
	raw[2]=(buf[5]<<8)+buf[4];//physical z
	for(i=0;i<3;i++)
	{
		data[i] = 0;
		for(j=0;j<3;j++)
		{
			data[i]+=raw[j]*adxl345_position_map[pos][i][j];
		}
		g = (float)data[i]*3.9;
		data[i]=g;
	}
	adxl345_data_filter(accel,data);
}
void adxl345_report(struct acceleration *accel)
{
	struct acceleration accel_origin;
	double magnitude=0;
	int tiltAngle;
	accel_origin.x = accel->x;
	accel_origin.y = accel->y;
	accel_origin.z = accel->z;
	Prints("Accelerometer_X=");
	if(accel->x<0)
	{
		uart_putchar('-');
		accel->x = -accel->x;
	}
	PrintInt(accel->x);
	uart_putchar('m');
	uart_putchar('g');
	_delay_ms(300);
	uart_putchar('\n');
	uart_putchar('\r');
	Prints("Accelerometer_Y=");
	if(accel->y<0)
	{
		uart_putchar('-');
		accel->y = -accel->y;
	}
	PrintInt(accel->y);
	uart_putchar('m');
	uart_putchar('g');
	_delay_ms(300);
	uart_putchar('\n');
	uart_putchar('\r');
	Prints("Accelerometer_Z=");
	if(accel->z<0)
	{
		uart_putchar('-');
		accel->z = -accel->z;
	}
	PrintInt(accel->z);
	uart_putchar('m');
	uart_putchar('g');
	_delay_ms(300);
	uart_putchar('\n');
	uart_putchar('\r');
	uart_putchar('\n');
	uart_putchar('\r');
	magnitude = (double)sqrt((double)(accel->x)*(accel->x)+(double)(accel->y)*(accel->y)+(double)(accel->z)*(accel->z));
	tiltAngle = (int)round(asin(accel_origin.z/magnitude)*180/3.141592);
	Prints("tiltAngle=");
	if(tiltAngle<0)
	{
		tiltAngle = -tiltAngle;
		uart_putchar('-');
	}
	PrintInt(tiltAngle);
	uart_putchar('\n');
	uart_putchar('\r');
	if(abs(tiltAngle) > MAX_TILT){

	}else{
		/*Calculate the orientation angle.
			This is the angle between the x-y projection of the up vector onto the +y-axis,increasing clockwise in a range[0,360]
			degrees.
		*/
		int orientationAngle = (int)round(atan2((double)accel_origin.y,(double)(-accel_origin.x))*180/3.141592);
		if(orientationAngle<0){
			/*atan2 returns [-180,180];normalize to [0,360]*/
			orientationAngle += 360;
		}
		Prints("orientationAngle=");
		PrintInt(orientationAngle);
		uart_putchar('\n');
		uart_putchar('\r');
		/*find the nearest rotation*/
		int nearestRotation = (orientationAngle +45)/90;
		if(nearestRotation == 4){
			nearestRotation = 0;
		}
		Prints("nearestRotation=");
		PrintInt(nearestRotation);
		uart_putchar('\n');
		uart_putchar('\r');
	}
}
int main()
{
	int update;
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
	adxl345 = malloc(sizeof(struct adxl345_data));
	adxl345->threadshold = Default_Threadshold;
	struct acceleration accel;
	while(1)
	{
		adxl345_measure(&accel);
		adxl345->last = accel;
		update = adxl345->update;
		if(update==1)
		{
			adxl345_report(&accel);
		}
	}	
	return 0;
}
