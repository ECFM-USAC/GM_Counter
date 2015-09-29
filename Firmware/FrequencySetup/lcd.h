#include <msp430g2553.h>
#include "ports.h"

#ifndef LCD_H_
#define LCD_H_

void lcd_delay(unsigned int k);
void data_write(void);
void data_read(void);
void send_command(unsigned char cmd);
void send_data(unsigned char data);
void lcd_print(char *s);
void lcd_print_number(unsigned long number);
void lcd_init(void);
void lcd_clear(void);
void lcd_gotoRow(unsigned int row);

#endif /* LCD_H_ */
