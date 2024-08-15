// main program
#include "uart.h"
#include "simpleshell.h"

void main()
{
    // set up serial console
    uart_init();
    
    // say hello
    // uart_puts("Hello World!\n"); // put(send) string to AUX_MU_IO register
    
    shell();
    // echo everything back 等待對方裝置傳送的char且ack相同char回去
    while(1) {
        uart_send(uart_getc()); 
    }
    
}