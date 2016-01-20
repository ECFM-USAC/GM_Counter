#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR = 0x00;
    ADC10CTL0 = 0x80; //Reference buffer enabled only during conversion. ADC OFF

	return 0;
}
