/* a properly aligned buffer */
extern volatile unsigned int mailbox[36];
#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags : VC,HW */
#define MBOX_TAG_REQUEST_CODE	           0x00000 // START TAG
/* videocore vc*/
#define MBOX_TAG_GET_FW_REVISION           0x00001        
/*Hardware*/
#define MBOX_TAG_GET_BOARD_MODEL           0X10001 
#define MBOX_TAG_GET_BOARD_REVISION        0X10002 
#define MBOX_TAG_GET_BOARD_MAC_ADDRESS     0X10003 
#define MBOX_TAG_GET_BOARD_SERIAL          0x10004
#define MBOX_TAG_GET_ARM_MEMORY            0x10005
#define MBOX_TAG_GET_VIDEOCORE_MEMORY      0x10006
#define MBOX_TAG_GET_CLOCKS                0x10007
#define MBOX_TAG_LAST           0x0 // END TAG

/*Config*/
#define MBOX_TAG_GET_COMMAND_LINE          0x50001

int mailbox_call();
void Get_board_revision();  // board revision
void Get_ARM_MEM(); //  ARM memory base address and size.