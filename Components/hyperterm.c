
#include <stdio.h>
#include <stdarg.h>
#include "common.h"
#include "hyperterm.h"
#include "uart.h"

#define HYPERTERM_UART  USE_UART0
#define HYPERTERM_FORMAT_BUFFER 15
#define HYPERTERM_SUBFORMAT_BUFFER 7


int hyperterm_echo = TRUE;


void hyperterm_init(void) {
  uart_switch(HYPERTERM_UART);
  uart_init();
}

void hyperterm_set_echo(int stat){
  hyperterm_echo = stat;
}

void hyperterm_putchar(char c) {
  uart_switch(HYPERTERM_UART);
  uart_put_byte(c);
}
char hyperterm_getchar(void){
  char byte;
  uart_switch(HYPERTERM_UART);
  byte = uart_get_byte();
  if(hyperterm_echo) {
    hyperterm_putchar(byte);
  }
  return byte;
}

char hyperterm_printf(char* format, ...) {
  char buffer[HYPERTERM_FORMAT_BUFFER];
  char sub_format[HYPERTERM_SUBFORMAT_BUFFER];
  char type;
  int j = 0;
  int i, arg_size;
  va_list args;
  va_start(args, format);

  for(j = 0; format[j] != '\0'; j++) {

    if(format[j] == '%' ) {
      arg_size = 0;
      for(i = 0; i < HYPERTERM_SUBFORMAT_BUFFER-1; i++) {
        if((format[j+i] >= '0' && format[j+i] <= '9') || format[i] == '.' || format[j+i] == '%') {
          arg_size++;
        }
        else break;
      }
      
      for(i = 0; i <= arg_size; i++) {
        sub_format[i] = format[j+i];
      }
      type = sub_format[i-1];
      sub_format[i] = '\0';
      j += arg_size;


      switch(type) {
        case '%':
          buffer[0] = '%';
          buffer[1] = '\0';
          break;
        case 'i':
        case 'd':
          sprintf(buffer, sub_format, va_arg(args, int));
          break;
        case 'c':
          sprintf(buffer, sub_format, va_arg(args, char));
          break;
        case 'x':
        case 'X':
          sprintf(buffer, sub_format, va_arg(args, int));
          for(i = 0; buffer[i] != '\0'; i++){
            buffer[i] = buffer[i]==' '?'0':buffer[i];
          }
          break;
        case 's':
          sprintf(buffer, sub_format, va_arg(args, char*));
          
          break;
      }

      for(i = 0; i < HYPERTERM_FORMAT_BUFFER && buffer[i] != '\0'; i++) {
        hyperterm_putchar(buffer[i]);
      }
    }
    else {
      hyperterm_putchar(format[j]);
    }
  }

  va_end(args);
}

void hyperterm_clear(void) {
    hyperterm_putchar(0xc);
    //hyperterm_putchar("\x1B[2J");
}

