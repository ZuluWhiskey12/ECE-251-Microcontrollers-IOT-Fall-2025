#include "ECE251_PIO_util.h"

/**
 * \brief Set up UART for using in ECE251 Labs.
 *
 * Sets up UART0 (on PA0 and PA1) with 8 data bits, no parity and one stop bit.
 *
 * @param[in] baudrate baud rate to configure UART with
 */
void UARTsetup(uint32_t baudrate)
{
	gpio_enable_ahb_aperture();

	/* Enable GPIOA in run mode. */
	periph_clock_enable(RCC_GPIOA);
	/* Mux PA0 and PA1 to UART0 (alternate function 1) */
	gpio_set_af(GPIOA, 1, GPIO0 | GPIO1);

	/* Enable the UART clock */
	periph_clock_enable(RCC_UART0);
	/* We need a brief delay before we can access UART config registers */
	__asm__("nop");
	/* Disable the UART while we mess with its setings */
	uart_disable(UART0);
	/* Configure the UART clock source as precision internal oscillator */
	uart_clock_from_piosc(UART0);
	/* Set communication parameters */
	uart_set_baudrate(UART0, baudrate);
	uart_set_databits(UART0, 8);
	uart_set_parity(UART0, UART_PARITY_NONE);
	uart_set_stopbits(UART0, 1);
	/* Now that we're done messing with the settings, enable the UART */
	uart_enable(UART0);
}

/**
 * \brief Write one character.
 *
 * Sends a single character to the UART. This will block until there is room
 * for the write to the UART.
 *
 * @param[in] char the character to send
 */
void UARTputc(char c) {
    uart_send_blocking(UART0, c);
}

/**
 * \brief Writes a string (null-terminated).
 *
 * Send a null-terminated string of characters to UART
 *
 * @param[in] buffer a pointer to the array of characters to send
 */
void UARTstring(char *buffer) {
    for (char* p=buffer; *p != '\0'; p++)
        uart_send_blocking(UART0, *p);
}

/**
 * \brief Writes a string (fixed length).
 *
 * Send a fixed number of characters from buffer to UART
 *
 * @param[in] buffer a pointer to the array of characters to send
 * @param[in] len number of characters to send
 */
int UARTwrite(char *buffer, uint32_t len) {
    uint32_t idx;
    for (idx = 0; idx < len; idx++)
        uart_send_blocking(UART0, buffer[idx]);
    return(idx);
}

#define ECE251_UART_MAX_STRING 256
/**
 * \brief Writes to UART like printf.
 *
 * Mimics operation of printf and sends arguments to UART. Note:
 * maximum number of characters output = 256.
 *
 * @param[in] format printf-like format string
 */
void UARTprintf(const char *format, ...) {
    char outstr[ECE251_UART_MAX_STRING];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(outstr, ECE251_UART_MAX_STRING, format, argptr);
    va_end(argptr);

    UARTstring(outstr);
}

/**
 * \brief Read one character.
 *
 * Reads a single character from the UART. This will block until there is
 * data to read from the UART.
 * @return single character from UART
 *
 */
unsigned char UARTgetc(void) {
    return(uart_recv_blocking(UART0));
}

/**
 * \brief Read one character and echo.
 *
 * Reads a single character from the UART. This will block until there is
 * data to read from the UART.
 * @return single character from UART
 *
 */
unsigned char UARTgetc_echo(void) {
	unsigned char c = uart_recv_blocking(UART0);
	UARTputc(c);
	return(c);
}

/**
 * \brief Sets up PORTF.
 *
 * Configures PORTF to drive the RGB LEDs and read the two user switches.
 *
 */
void GPIOF_setup(void)
{
	/*
	 * Configure GPIOF
	 * This port is used to control the RGB LED
	 */
	periph_clock_enable(RCC_GPIOF);
	const uint32_t outpins = (LED_R | LED_G | LED_B);

	gpio_mode_setup(RGB_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, outpins);
	gpio_set_output_config(RGB_PORT, GPIO_OTYPE_PP, GPIO_DRIVE_2MA, outpins);

	/*
	 * Now take care of our buttons
	 */
	const uint32_t btnpins = USR_SW1 | USR_SW2;

	/*
	 * PF0 is a locked by default. We need to unlock it before we can
	 * re-purpose it as a GPIO pin.
	 */
	gpio_unlock_commit(GPIOF, USR_SW2);
	/* Configure pins as inputs, with pull-up. */
	gpio_mode_setup(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, btnpins);
}
