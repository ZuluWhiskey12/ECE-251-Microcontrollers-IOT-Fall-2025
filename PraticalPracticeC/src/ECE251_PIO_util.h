#include <libopencm3/lm4f/systemcontrol.h>
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/uart.h>
#include <stdio.h>
#include <stdarg.h>

#define RGB_PORT	GPIOF
enum {
	LED_R	= GPIO1,
	LED_G	= GPIO3,
	LED_B	= GPIO2,
};

/* This is how the user switches are connected to GPIOF */
enum {
	USR_SW1	= GPIO4,
	USR_SW2	= GPIO0,
};

void UARTsetup(uint32_t);
void UARTputc(char);
void UARTstring(char*);
int UARTwrite(char*, uint32_t);
void UARTprintf(const char*, ...);
unsigned char UARTgetc(void);
unsigned char UARTgetc_echo(void);
void GPIOF_setup(void);