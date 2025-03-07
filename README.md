Codes are based on NYCU OSC class's lab with some modifications and comments.
Some of them are clean codes from [前人修過課的學長的筆記](https://github.com/KevinCodePlace/NYCU_Operating_System_Capstone_2023Spring).
Read, understand and practice OS, C, bitwise operations, pointer and implement it by reading spec.

# Environment
```
BCM2837 
wsl Ubuntu cross compile
Board               Rpi3  
AArch64	            Yes  
PartNum	            0xD03  
MMIO base address   0x3F000000  
``` 
# ARM booting steps
```
When poweron, cpu is off and SDRAM disable, we need to use GPU start booting.  
1.GPU goto ROM and execute first step bootloader. GPU read bootcode.bin  
2.second steps bootcode.bin do enable SDRAM and load&execute start.elf  
3.ELF read kernel.img, config.txt, cmdline.txt and load them into memory then wake up ARM kernel.    
```
# a.S startup code (without bootloader code which  check cpu status and continue to run main)
```
1. _start: 0x80000 which simpleshell.c start
mrs x1, mpidr_el1 save cpu information into x1  
and x1, x1, #3 (x1 and 11) get 2bits represented cpu state   
cbz x1, setting if x1 == 0 then branch setting
2. halt :
3. setting : intitialize stack pointer sp = _start and initialize bss
4. clear_bss : initialize bss
5. kernel_main: run main program
```

# Linker.ld
```
. = 0x80000; // start at 0x80000 without bootloader
// in new version will setup new memory mapping and add bootloader into kernel image
```

# Address NOTE
```
Software directly accessing peripherals must translate these addresses into
physical or virtual addresses, as described above.
Software accessing peripherals using the DMA engines must use bus addresses. 
```

# GPIO.h  
The bus addresses for peripherals are set up to map onto
the peripheral bus address range starting at `0x7E000000`.  
The MMU maps physical address `0x3f000000` to bus address `0x7e000000`.

Here I use direct mapping to physical address  
`MMIO_BASE` `0x3F00 0000`  

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
# Mailbox
[Mailbox interface](https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface)
```
int mailbox_call() // make a mailbox call. Returns 0 on failure, non-zero on success
void Get_board_revision()
void Get_ARM_MEM()
```
```
/* mailbox message buffer */
example:
    mailbox[0] = 7 * 4; // (size in bytes including the header values, the end tag and padding)
    mailbox[1] = MBOX_REQUEST;   // buffer request
    mailbox[2] = MBOX_TAG_GET_BOARD_REVISION; // tag id
    mailbox[3] = 4; // maximum of request and response value buffer's length.
    mailbox[4] = MBOX_TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer 
    mailbox[6] = MBOX_TAG_LAST; // tags end
```
