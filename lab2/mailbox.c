#include "gpio.h"
#include "mailbox.h"
/* mailbox message buffer */
volatile unsigned int  __attribute__((aligned(16))) mailbox[36]; // linker script aligh 16bits

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mailbox_call()
{
    unsigned int r = (((unsigned int)((unsigned long)&mailbox)&~0xF) | (MBOX_CH_PROP&0xF));
    /* wait until we can write to the mailbox */
    do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_FULL);
    /* write the address of our message to the mailbox with channel identifier */
    *MBOX_WRITE = r;
    /* now wait for the response */
    while(1) {
        /* is there a response? */
        do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_EMPTY);
        /* is it a response to our message? */
        if(r == *MBOX_READ)
            /* is it a valid successful response? */
            return mailbox[1]==MBOX_RESPONSE;
    }
    return 0;
}


void Get_board_revision() {
    mailbox[0] = 7 * 4;
    mailbox[1] = MBOX_REQUEST;   
    mailbox[2] = MBOX_TAG_GET_BOARD_REVISION;
    mailbox[3] = 4;
    mailbox[4] = MBOX_TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = MBOX_TAG_LAST;
    mailbox_call();
}

void Get_ARM_MEM() {
    mailbox[0] = 8 * 4;
    mailbox[1] = MBOX_REQUEST;   
    mailbox[2] = MBOX_TAG_GET_ARM_MEMORY;
    mailbox[3] = 8;
    mailbox[4] = MBOX_TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = 0; // value buffer
    mailbox[7] = MBOX_TAG_LAST;
    mailbox_call();
}