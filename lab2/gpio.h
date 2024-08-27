/*

GPIO pins should be set up first the before enabling the UART. The UART core is build to 
emulate 16550 behaviour. So when it is enabled any data at the inputs will immediately be 
received . If the UART1_RX line is low (because the GPIO pins have not been set-up yet) 
that will be seen as a start bit and the UART will start receiving 0x00-characters.  

Physical addresses range from 0x3F000000 to 0x3FFFFFFF for peripherals. 
The bus addresses for peripherals are set up to map onto
the peripheral bus address range starting at 0x7E000000.

Thus a peripheral advertised here at bus address 0x7Ennnnnn is
available at physical address 0x3Fnnnnnn

However, the reference uses bus addresses. "You should translate them into physical one."

0x3F000000 mapping 0x7E000000
The MMU maps physical address 0x3f000000 to bus address 0x7e000000.
For more, see https://nycu-caslab.github.io/OSC2024/labs/hardware/uart.html#uart
*/

#define MMIO_BASE       0x3F000000
#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000)) // GPIO Function Select 0 R/W
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004)) // GPIO Function Select 1 R/W
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008)) // GPIO Function Select 2 R/W
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C)) // GPIO Function Select 3 R/W
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010)) // GPIO Function Select 4 R/W
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014)) // GPIO Function Select 5 R/W
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C)) // GPIO Pin Output Set 0  W
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020)) // GPIO Pin Output Set 1  W
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028)) // GPIO Pin Output Clear 0 W
#define GPCLR1          ((volatile unsigned int*)(MMIO_BASE+0x0020002C)) // GPIO Pin Output Clear 1 W
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034)) // GPIO Pin Level 0 R
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038)) // GPIO Pin Level 1 R
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040)) // GPIO Pin Event Detect Status 0 R/W
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044)) // GPIO Pin Event Detect Status 1 R/W
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064)) // GPIO Pin High Detect Enable 0 R/W 
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068)) // GPIO Pin High Detect Enable 1 R/W
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094)) // GPIO Pin Pull-up/down Enable R/W
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098)) // GPIO Pin Pull-up/down Enable Clock 0
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C)) // GPIO Pin Pull-up/down Enable Clock 1