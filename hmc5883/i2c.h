#ifndef __i2c_h__
#define __i2c_h__
#include<inttypes.h>
void i2c_init();
int read_bytes(uint16_t eeaddr,int len,uint8_t *buf);
int write_bytes(uint16_t eeaddr,int len,uint8_t *buf);
#endif
