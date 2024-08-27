/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
 
void uart_init();

/**
 * Send a character
 */
void uart_send(unsigned int c);

/**
 * Receive a character
 */

char uart_getc();

/**
 * Display a string
 */
void uart_puts(char *s);


/**
 * Display a binary value in hexadecimal
 */
void uart_hex(unsigned int d);