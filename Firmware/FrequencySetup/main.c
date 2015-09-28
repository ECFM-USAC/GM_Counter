#include <msp430g2553.h>
#include "ports.h" //Port definitions
#include "setup.h" //Init functions
#include "lcd.h"   //LCD driver
#include "timing.h" //Timer drivers
#include "gmCore.h" //GM high-level functions
#include <string.h>

unsigned int hvFrequency = 300;


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	ioSetup();
	clkSetup(16); //Set MCLK to 16 MHz. SMCLK is set with a 1:8 prescaler
	timerSetup();
	interruptSetup();
	//setHVFrequency(32); //---- This is a simple test. Set HV frequency here during evaluation
	lcd_init();
	lcd_print("ECFM - USAC");
	lcd_gotoRow(2);
	lcd_print("Geiger-Mõller"); //Geiger-Müller
	while(1){
		setHVFrequency(hvFrequency);
		lcd_clear();
		lcd_print("f = ");
		lcd_print_number(hvFrequency); //----------------- How to convert integers to string????????????
		lcd_print(" Hz");

		_BIS_SR(LPM0);
	}
}

//---------------------- CONVERT TO ANODE SENSING GM ------------------------------------

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
	if(P1IFG & BIT3){ //PUSH BUTTON
		P1IFG &= ~PUSH_BTN;
		hvFrequency += 20;
		_BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM0_EXIT);
	}
}


//TIMER0_A0 Interrupts aren't used to toggle HV pin any more
/*
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TACCR0_ISR(void){<
    //CCR0 used for HV frequency generator
    //if(TACCTL0 & CCIFG){ //TACCTL0 Interrupt Flag
    if(CCIFG){ //TACCTL0 Interrupt Flag
        //P1OUT ^= FREQ_OUT; //Toggle corresponding pin
        ++cnt;
    }
    //CCIFG is automatically cleared
}
*/
