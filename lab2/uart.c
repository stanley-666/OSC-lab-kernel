#include "gpio.h"
// Board	AArch64	    PartNum	    MMIO base address
// Rpi3	    Yes	        0xD03	        0x3F000000
/* Auxilary mini UART registers */
/*
Use physical memory, so based on pdf data sheet 
Auxiliary peripherals Register Map 
(offset = 0x7E21 5000) 
Address         Register Name3      Description             Size 
0x7E21 5004     AUX_ENABLES         Auxiliary enables       3 
ex. bus address 0x7E21 5004 map to MMIO base address + 0x00215004
*/
/* define register mapping detail started from page 11 */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004)) // Auxiliary enables   size 3
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040)) // Mini Uart I/O Data   size 8
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044)) // Mini Uart Interrupt Enable   size 8
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048)) // Mini Uart Interrupt Identify   size 8
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C)) // Mini Uart Line Control    size 8
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050)) // Mini Uart Modem Control    size 8
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054)) // Mini Uart Line Status    size 8
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058)) // Mini Uart Modem Status   size 8
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C)) // Mini Uart Scratch   size 8
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060)) // Mini Uart Extra Control   size 8
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064)) // Mini Uart Extra Status   size 32
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068)) // Mini Uart Baudrate   size 16

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO

 The mini Uart has the following features: 
• 7 or 8 bit operation. 
• 1 start and 1 stop bit. 
• No parities. 
• Break generation. 
• 8 symbols deep FIFOs for receive and transmit. 
• SW controlled RTS, SW readable CTS. 
• Auto flow control with programmable FIFO level. 
• 16550 like registers. 
• Baudrate derived from system clock. 
This is a mini UART and it does NOT have the following capabilities: 
• Break detection 
• Framing errors detection. 
• Parity bit 
• Receive Time-out interrupt 
• DCD, DSR, DTR or RI signals. 
*/
void uart_init()
{
    register unsigned int r;

    /* initialize UART */
    *AUX_ENABLE |=1;    // enable UART1, AUX mini uart Then mini UART register can be accessed.  0x????????or 0x00000001 = 1
    *AUX_MU_CNTL = 0;   // Disable transmitter and receiver during configuration.
    *AUX_MU_LCR = 3;    // 8 bits mode (mini uart have 7/8 bits mode)
    *AUX_MU_MCR = 0;    // Don’t need auto flow control.
    *AUX_MU_IER = 0;    // Disable interrupt because currently you don’t need interrupt.
    *AUX_MU_IIR = 6;    // disable interrupts and FIFO
    /*Scratch provided by the SDC */
    *AUX_MU_BAUD = 270;    // Set baud rate to 115200

    /* map UART1 to GPIO pins */
    r=*GPFSEL1;
    r&=~((7<<12)|(7<<15)); // gpio14, gpio15
    r|=(2<<12)|(2<<15);    // alt5
    *GPFSEL1 = r;
    *GPPUD = 0;            // GPIO pin enable pins 14 and 15
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;        // turnoff clock signal prevent from changing pin state (flush GPIO setup)
    *AUX_MU_CNTL = 3;      // enable Tx, Rx transmitter and receiver
}

/**
 * Send a character
 */
void uart_send(unsigned int c) {
    /* wait until we can send */
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20)); // wait till Transmitter buffer idle,  Transmitter empty緩衝區空了可以發送新的資料
    /* write the character c to the buffer */
    *AUX_MU_IO=c; // 收到要輸入的值c放入IO暫存器
}

/**
 * Receive a character
 */
char uart_getc() {
    char r;
    /* wait until something is in the buffer */

    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01)); // return 1 stand that Data ready field holds at least 1 symbol 表示緩衝區有資料可以讀了
    /*
    Read data
    Check AUX_MU_LSR_REG’s data ready field.
    If set, read from AUX_MU_IO_REG
    */ 
    /* read it and return */
    r=(char)(*AUX_MU_IO); // get char data from IO register
    /* convert carriage return to newline */
    /* if r == '\r' return '\n' else return r */
    return r=='\r'?'\n':r;
}

/**
 * Display a string
 */
void uart_puts(char *s) {
    while(*s) {
        /* convert newline to carriage return + newline */
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
}