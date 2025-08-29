/*
 * ECE251 LAB1
 *
 * This is an example C program to use in the first Lab.
 * 
 */

#include "ECE251_PIO_util.h"

void delay(void);

#define FLASH_DELAY 1000000
void delay(void) {
	int i;
	for (i = 0; i < FLASH_DELAY; i++)       /* Wait a bit. */
		__asm__("nop");
}

int main(void)
{
	UARTsetup(9600); // Setup the UART
	GPIOF_setup(); // Configure LEDs and buttons

    UARTprintf("Hello from TIVA!!!\n\n");

	int v = 0;
	for (int i = 0; i < 10; i++) {
		v = v + i;
		UARTprintf("i: %d | v: %X\n", i, v);
	}

	char *b = (char *)0x20000400;
	for (int i = 0; i < 32; i++) {
		*(b + i) = 0;
	}
	for (int i = 0; i < 32; i++) {
		*(b + i) = i;
	}

	while(1) {
		/*
		 * Flash the Red diode
		 */
		gpio_set(RGB_PORT, LED_R);
		delay(); /* Wait a bit. */
		gpio_clear(RGB_PORT, LED_R);
		delay(); /* Wait a bit. */

		/*
		 * Flash the Green diode
		 */
		gpio_set(RGB_PORT, LED_G);
		delay(); /* Wait a bit. */
		gpio_clear(RGB_PORT, LED_G);
		delay(); /* Wait a bit. */

		/*
		 * Flash the Blue diode
		 */
		gpio_set(RGB_PORT, LED_B);
		delay(); /* Wait a bit. */
		gpio_clear(RGB_PORT, LED_B);
		delay(); /* Wait a bit. */
	}

	// we should never get here
	return 0;
}