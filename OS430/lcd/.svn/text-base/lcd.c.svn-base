#include <stdio.h>
#include <stdarg.h>
#include "lcd.h"
#include <msp430x14x.h>

#define LCD_FORMAT_BUFFER 15
#define LCD_SUBFORMAT_BUFFER 7
#define TRUE 1
#define FALSE 0

#define LCD_DL 0 //4-bit mode
#define LCD_N  1 //2-line mode
#define LCD_F  1 //display ON
#define LCD_D  1 //display ON
#define LCD_C  0 //cursor OFF
#define LCD_B  0 //cursor blink ON
#define LCD_SH 0 //Entire Shift on
#define LCD_ID 1 //increment ON

#define LCD_E   0x40          //Enable is plugged in P6.6
#define LCD_RS  0x20         //Register Select is plugged in P6.5
#define LCD_RW  0x10          // Read/Write plugged in P6.4

#define LCD_DB4_0   0x01      //DB4/DB0 is plugged in P6.0
#define LCD_DB5_1   0x02     //DB5/DB1 is plugged in P6.1
#define LCD_DB6_2   0x04      //DB6/DB2 is plugged in P6.2
#define LCD_DB7_3   0x08      //DB7/DB3 is plugged in P6.3

#define LINE_1_ADDRESS 0x00
#define LINE_2_ADDRESS 0x40

char currentCursor;

void lcd_init(void){
  //char status = 0x80;
  //timer_wait(2000); //After power up wait 40ms

  P6DIR = 0xFF; //everybody OUT !
  P6SEL = 0x00; //select GPIO mode for everything
  
  P6OUT = 0x00;
  
  //timer_wait(4800);


  P6OUT = LCD_DB5_1 + LCD_DB4_0; 
  lcd_pulse_e();
  P6OUT = LCD_DB5_1 + LCD_DB4_0; 
  lcd_pulse_e();
  P6OUT = LCD_DB5_1 + LCD_DB4_0; 
  lcd_pulse_e();

  /*Function Set*/
  P6OUT = LCD_DB5_1; 
  lcd_pulse_e();

  P6OUT = LCD_DB5_1;
  lcd_pulse_e();

  P6OUT = LCD_DB7_3*LCD_N + LCD_DB6_2*LCD_F;
  lcd_pulse_e();
 
  //timer_wait(480);  //wait 600us
  
  /*Display ON/OFF Control*/
  P6OUT = 0x00;
  lcd_pulse_e();
  P6OUT = LCD_DB7_3 + LCD_DB6_2*LCD_D + LCD_DB5_1*LCD_C + LCD_DB4_0*LCD_B;
  lcd_pulse_e();

 //timer_wait(480);

  /*Display Clear*/
  P6OUT = 0x00;
  lcd_pulse_e();
  ////timer_wait(480);  //wait 600us
  P6OUT = LCD_DB4_0;
  lcd_pulse_e();

 //timer_wait(480);

  /*Entry Mode Set*/
  P6OUT = 0x00;
  lcd_pulse_e();
  P6OUT = LCD_DB6_2 + LCD_DB5_1*LCD_ID + LCD_DB4_0*LCD_SH;
  lcd_pulse_e();

  //timer_wait(480);//Initialization End

  currentCursor = 0;
  //P6OUT =0x00;
  //lcd_pulse_e();

  //lcd_read_status();
  /*while(status&0x80 != 0){
  status = lcd_read_status();
  } */

  //lcd_writebyte('A');

}

void lcd_writebyte(char b ){
  char status = 0x80;
  P6DIR = 0xFF;
  P6OUT = 0x00;
  P6OUT &= ~LCD_RS;
  P6OUT &= ~LCD_RW;

  P6OUT = LCD_DB7_3;
  lcd_pulse_e();

  P6OUT = 0x00;
  lcd_pulse_e();

  P6OUT |= LCD_RS;
  P6OUT &= ~LCD_RW;
  P6OUT &= ~0x0F;

  P6OUT |= (b >> 4)&0x0F; // first four bits are db7,db6,db5,db4
  lcd_pulse_e();

  P6OUT &= ~0x0F;

  P6OUT |= b & 0x0F;
  lcd_pulse_e();


  //while(status&0x80 != 0){
  //status = lcd_read_status();
  //} 
}

char lcd_read_status(void){
  char rb = 0;
  P6DIR = LCD_RS + LCD_RW + LCD_E; //only those three are out values, rest is IN
  P6OUT = LCD_RW; //set read pin high

  lcd_pulse_e();
  rb += (P6IN&0x0F) << 4;

  lcd_pulse_e();
  rb += (P6IN&0x0F);

  P6DIR = 0xFF; //put it back to ALL OUT
  return rb;
}


void lcd_clearscreen(void){
  P6OUT &= ~LCD_RS;
  P6OUT &= ~LCD_RW;
  send_data(0x01);
  currentCursor = 0;
  //timer_wait(500);
}

void lcd_pulse_e(void)
{
  //wireless_receive_status(FALSE);
  //set e high
  P6OUT |= LCD_E;
 
  //pause
  //timer_wait(3);
 
  //set e low
  P6OUT &= ~LCD_E;
 
  //pause
  //timer_wait(3);
  //wireless_receive_status(TRUE);
} 

void send_data(unsigned char data)
{
  //send high part first
  send_nibble((data>>4) & 0x0F);
  send_nibble(data & 0x0F);
}

void send_nibble(unsigned char data)
{
  //convert the nibble to the appropriate pins
  if(data & 0x01)
    P6OUT |= LCD_DB4_0;
  else
    P6OUT &= ~LCD_DB4_0;
 
  if(data & 0x02)
    P6OUT |= LCD_DB5_1;
  else
    P6OUT &= ~LCD_DB5_1;
 
  if(data & 0x04)
    P6OUT |= LCD_DB6_2;
  else
    P6OUT &= ~LCD_DB6_2;
 
  if(data & 0x08)
    P6OUT |= LCD_DB7_3;
  else
    P6OUT &= ~LCD_DB7_3;
 
  lcd_pulse_e();
} 

  
void lcd_goto(char p){
  P6OUT &= ~LCD_RS;
  P6OUT &= ~LCD_RW;
  if (p < 24 ) send_data(0x80 | (LINE_1_ADDRESS + p));
  else if (p < 48 ) send_data(0x80 | (LINE_2_ADDRESS + (p-24)));
  currentCursor = p;
}


void lcd_putchar(char c){
  lcd_goto(currentCursor);
  currentCursor++;
  P6OUT |= LCD_RS;
  P6OUT &= ~LCD_RW;
  send_data(c);
}
void printString(char* string)
{
  while ((*string) != '\0'){
    lcd_putchar(*string);
    string++;
  }
} 

char lcd_printf(char* format, ...) {
  char buffer[LCD_FORMAT_BUFFER];
  char sub_format[LCD_SUBFORMAT_BUFFER];
  char type;
  int j = 0;
  int i, arg_size;
  va_list args;
  va_start(args, format);

  for(j = 0; format[j] != '\0'; j++) {

    if(format[j] == '%' ) {
      arg_size = 0;
      for(i = 0; i < LCD_SUBFORMAT_BUFFER-1; i++) {
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
          sprintf(buffer, sub_format, va_arg(args, int));
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

      for(i = 0; i < LCD_FORMAT_BUFFER && buffer[i] != '\0'; i++) {
        lcd_putchar(buffer[i]);
      }
    }
    else {
      lcd_putchar(format[j]);
    }
  }

  va_end(args);
}

int lcd_checkAlphabet(char key){
  if (key >= '0' && key <= '9') return TRUE;
  if (key >= 'A' && key <= 'Z') return TRUE;
  if (key >= 'a' && key <= 'z') return TRUE;
  return FALSE;
}

void lcd_changeline(void){
  if (currentCursor >23) lcd_goto(LCD_LINE1);
  else lcd_goto(LCD_LINE2);
}


void lcd_clearline(int line){
  if(line == 1){
    lcd_goto(LCD_LINE1);
    lcd_printf("                        ");
    lcd_goto(LCD_LINE1);

  }
  if(line == 2){
    lcd_goto(LCD_LINE2);
    lcd_printf("                        ");
    lcd_goto(LCD_LINE2);
  }


}

