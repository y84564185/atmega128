#ifndef __uart_h__
#define __uart_h__
#include<inttypes.h>
void uart_init(void);
void uart_putchar(char c);
char uart_getchar(void);
void Prints(uint8_t *str);
void PrintUchar(uint8_t c);
void PrintInt(uint16_t c);
void PrintLongInt(uint32_t c);
#endif

