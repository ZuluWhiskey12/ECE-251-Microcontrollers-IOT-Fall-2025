//*****************************************************************************
// Lab 1 - Simple hello world example
//*****************************************************************************

#include "ECE251_PIO_util.h"
#define FLASH_DELAY 500000
void delay(void);

//*****************************************************************************
// Print "Hello World!" to the UART on the evaluation board.
//*****************************************************************************
void delay(void) {
	int i;
	for (i = 0; i < FLASH_DELAY; i++)       /* Wait a bit. */
		__asm__("nop");
}

int main(void)
{
  
	UARTsetup(9600); // Setup the UART
	GPIOF_setup(); // Configure LEDs and buttons

  // Hello!
  UARTprintf("Hello, world! Part 2\n");
    
  // We are finished. Hang around toggling LED.
  while(1)
  {
    // Turn on the BLUE and RED LED.
		gpio_set(RGB_PORT, LED_R | LED_B);
		delay(); /* Wait a bit. */

    // Turn off the BLUE and RED LED.
    gpio_clear(RGB_PORT, LED_R | LED_B);
		delay(); /* Wait a bit. */
  }
}
