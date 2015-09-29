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

	P2OUT = (P2OUT & 0xF0) + (command[0]);
	data_write();
	P2OUT = (P2OUT & 0xF0) + (command[1]);
	data_write();

}

void send_data(unsigned char cmd){
	unsigned int data[2];
	data[0] = (cmd>>4) & 0x0F;
	data[1] = cmd & 0x0F;

    P1OUT |=  LCD_RS; //Send data

    P2OUT = (P2OUT & 0xF0) + (data[0]);
	data_write();
	P2OUT = (P2OUT & 0xF0) + (data[1]);
	data_write();
}

void lcd_print(char *s){
    while(*s){
        send_data(*s++);
    }
}

void lcd_print_number(unsigned long number){
	unsigned long n = number;
	unsigned int nOut[16]; //At most 16 digits per number
	unsigned int cnt = 0;
	int i;
	while(n>0){
		nOut[cnt++] = n%10;
		n /= 10;
	}
	for(i = --cnt; i >= 0; i--){
		send_data(nOut[i]+48);
	}
}

void lcd_gotoRow(unsigned int row){ //Row 1 or 2
	switch(row){
	case 2:
		send_command(0xc0);
		break;

	default:
		send_command(0x80);
	}
}

void lcd_clear(void){
	send_command(0x0E); // clear the screen
	send_command(0x01); // display on cursor on
	send_command(0x06); // increment cursor
	lcd_gotoRow(1);     // row 1 column 1
}

void lcd_init(void){
    send_command(0x33);
    send_command(0x32);
    send_command(0x28); // 4 bit mode
    lcd_clear();
}
