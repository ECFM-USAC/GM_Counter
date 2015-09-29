#include <msp430g2553.h>
#include "ports.h" //Port definitions
#include "setup.h" //Init functions
#include "lcd.h"   //LCD driver
#include "timing.h" //Timer drivers
#include "gmCore.h" //GM high-level functions
#include "hvValues.h" //HV-freq table

unsigned int hvFrequency = START_FREQUENCY;

unsigned int setHvFlag = 1;
unsigned int setCountFlag = 1;

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
	lcd_print("Geiger-Mõller"); //Geiger-Mõller
	while(1){
		if(setHvFlag){
			setHVFrequency(hvFrequency);
			lcd_clear();
			lcd_print("f = ");
			lcd_print_number(hvFrequency);
			lcd_print(" Hz");
			setHvFlag = 0;
			setCountFlag = 1;
		}
		if(setCountFlag){
			lcd_gotoRow(2);
			lcd_print("Counts: ");
			lcd_print_number(pulseCount);
			setCountFlag = 0;
		}
		_BIS_SR(LPM1);
	}
}

//---------------------- CONVERT TO ANODE SENSING GM ------------------------------------

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
	if(P1IFG & BIT3){ //PUSH BUTTON
		P1IFG &= ~PUSH_BTN;
		hvFrequency = (hvFrequency + STEP_FREQUENCY) % (STOP_FREQUENCY);
		setHvFlag = 1;
		_BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM1_EXIT);
	}
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & BIT4){ //Pulse in
        P2IFG &= ~PULSE_IN;
        setCountFlag = 1;
        pulseCount++;
        soundTick();
        _BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM1_EXIT);

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

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TA1CCR2_ISR(void){
    if(TA1CCTL2 & CCIFG){
    //if(TA1IV & BIT4){ //TA1CCR2 IFG
        if(tickCount < CYCLES_PER_TICK){
            tickCount++;
        }else{
            soundOff();
            _BIC_SR(LPM1_EXIT);
        }
    }
}
