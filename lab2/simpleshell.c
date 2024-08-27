// simpleshell.c
#include "uart.h"
#include "string.h"
#include "mailbox.h"
#include "reboot.h"

/*.global*/
#define BUFFER_MAX_SIZE 10

void read_command(char* buffer);
void parse_command(char* buffer);

void shell() {
  while (1) {
    char buffer[BUFFER_MAX_SIZE];

    // Read command from target
    read_command(buffer);

    // Parse and execute command on host
    parse_command(buffer);
  }
}

void read_command(char* buffer) {
  int index = 0;
  // Print prompt
  //uart_puts("[Waiting target's command]\n");
  uart_puts("# ");
  while (1) {
    buffer[index] = uart_getc(); // 一個一個字元get 使用者之輸入存入buffer
    //uart_puts("send back :");
    uart_send(buffer[index]); // get到的字元send回去且顯示字元在terminal (-serial stdio)
    if (buffer[index] == '\n') {
      buffer[index] = '\0';
      buffer[index + 1] = '\n';
      break;
    }
    index++;
  }
}

void parse_command(char* buffer) {
  char* input_string = buffer;
 
  //uart_puts("[Parsing command from host]\n");
  if (strcmp(input_string, "help") == 0) { // command
    //uart_puts("[Responding from host]\n");
    uart_puts("help  : list available commands\n");
    uart_puts("hello : print Hello World!\n");
    uart_puts("info	: Get the hardware's information\n");
    uart_puts("reboot	: reboot the device\n");
    // Add more commands here
  }
  else if (strcmp(input_string, "hello") == 0) {
    uart_puts("Hello World!\n");
  }
  else if (strcmp(input_string, "reboot") == 0 ) {
    uart_puts("reboot the device\n");
    reset(1000);
  }
  else if(strcmp(input_string, "info") == 0 ) {
         if (mailbox_call()) {
           Get_board_revision();
           uart_puts("Board Revision = ");
           uart_hex(mailbox[5]);
           uart_puts("\r\n");
           Get_ARM_MEM();
           uart_puts("ARM memory base address = ");
           uart_hex(mailbox[5]);
           uart_puts("\r\n");
           uart_puts("ARM memory size = ");
           uart_hex(mailbox[6]);
           uart_puts("\r\n");
         }
  }
  else {
    uart_puts("Unknown command\n");
  }
}
