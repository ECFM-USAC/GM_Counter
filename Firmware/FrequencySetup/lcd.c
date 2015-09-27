/*
 * lcd.c
 *
 *  Created on: 27/09/2015
 *      Author: Ivan
 */

#include "lcd.h"

unsigned int i;
unsigned int j;


void lcd_delay(unsigned int k){
    for(j=0;j<k;j++){
        for(i=0;i<100*16;i++);
    }
}

void data_write(void){
    P1OUT |=  LCD_EN;
    lcd_delay(2);
    P1OUT &= ~LCD_EN;
}


void send_command(unsigned char cmd){
    unsigned int command[2];
    command[0] = (cmd>>4) & 0x0F;
    command[1] = cmd & 0x0F;

    P1OUT &= ~LCD_RS; //Send command

	P2OUT = (P2OUT & 0xF0) | (command[0]);
	data_write();
	P2OUT = (P2OUT & 0xF0) | (command[1]);
	data_write();

}

void send_data(unsigned char cmd){
	unsigned int data[2];
	data[0] = (cmd>>4) & 0x0F;
	data[1] = cmd & 0x0F;

    P1OUT |=  LCD_RS; //Send data

    P2OUT = (P2OUT & 0xF0) | (data[0]);
	data_write();
	P2OUT = (P2OUT & 0xF0) | (data[1]);
	data_write();
}

void send_string(char *s){
    while(*s){
        send_data(*s++);
    }
}

void lcd_clear(void){
	send_command(0x0E); // clear the screen
	send_command(0x01); // display on cursor on
	send_command(0x06); // increment cursor
	send_command(0x80); // row 1 column 1
}

void lcd_init(void){
    send_command(0x33);
    send_command(0x32);
    send_command(0x28); // 4 bit mode
    lcd_clear();
}
