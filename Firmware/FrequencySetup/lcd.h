/*
 * lcd.h
 *
 *  Created on: 27/09/2015
 *      Author: Ivan
 */

#include <msp430g2553.h>
#include "ports.h"

#ifndef LCD_H_
#define LCD_H_

void lcd_delay(unsigned int k);
void data_write(void);
void data_read(void);
void send_command(unsigned char cmd);
void send_data(unsigned char data);
void send_string(char *s);
void lcd_init(void);
void lcd_clear(void);

#endif /* LCD_H_ */
