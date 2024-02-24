/**
 * LCD header file with address constants to call in the lcd_goto() function
 *
 */

#ifndef __INCLUDE_LCD_H
#define __INCLUDE_LCD_H

#define LCD_LINE1 0
#define LCD_LINE2 24

void lcd_init(void);
void lcd_putchar(char c);
char lcd_read_status(void);
void printString(char* string);
void lcd_goto(char p);
void lcd_pulse_e(void);
void send_nibble(unsigned char data);
void send_data(unsigned char data);
void lcd_clearscreen(void);
char lcd_printf(char* format, ...);
int lcd_checkAlphabet(char key);
void lcd_changeline(void);
void lcd_clearline(int line);
#endif

