# Environment
```
wsl Ubuntu cross compile
Board               Rpi3  
AArch64	            Yes  
PartNum	            0xD03  
MMIO base address   0x3F000000  
``` 


# GPIO.h  


# Mini Uart   
##  The mini Uart has the following features: 
```
• 7 or 8 bit operation. 
• 1 start and 1 stop bit. 
• No parities. 
• Break generation. 
• 8 symbols deep FIFOs for receive and transmit. 
• SW controlled RTS, SW readable CTS. 
• Auto flow control with programmable FIFO level. 
• 16550 like registers. 
• Baudrate derived from system clock.
```

## This is a mini UART and it does NOT have the following capabilities:
```
• Break detection 
• Framing errors detection. 
• Parity bit 
• Receive Time-out interrupt 
• DCD, DSR, DTR or RI signals. 
```

## Auxilary mini UART registers  setting
`AUX_MU_IO` represent `AUX_MU_IO_REG register` which is primary used to `write data to` and `read data from` the 
`UART FIFOs`.

```
void uart_init()
void uart_send(unsigned int c) // send a character to IO register (AUX_MU_IO)
char uart_getc() // Receive a character from IO register (AUX_MU_IO)
void uart_puts(char *s) // Display a string
void uart_hex(unsigned int d) // convert character to hex

```
