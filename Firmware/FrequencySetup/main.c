/*
 * ToDo LIST
 *  - Implement timebase
 *  - Debounce push buttons using WDT as timer
 * 	- Convert to anode sensing
 * 	- Measure HV stage
 * 	- Implement a PID to control HV
 * 	- Control LCD's LED brightness (or at least, turn it on/off)
*/



#include <msp430g2553.h>
#include "ports.h" //Port definitions
#include "setup.h" //Init functions
#include "lcd.h"   //LCD driver
#include "timing.h" //Timer drivers
#include "gmCore.h" //GM high-level functions
#include "hvValues.h" //HV-freq table

volatile unsigned int hvFrequency;

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
	lcd_print("Geiger-M�ller"); //Geiger-M�ller

	hvFrequency = START_FREQUENCY;

	P1DIR |= BIT7;

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
			//lcd_print("Counts: ");
			//lcd_print_number(pulseCount);
			lcd_print("CPM: ");
			lcd_print_number(cpm);
			setCountFlag = 0;
		}
		_BIS_SR(LPM1);
	}
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
	if(P1IFG & BIT3){ //PUSH BUTTON
		P1IFG &= ~PUSH_BTN;
		hvFrequency = (hvFrequency + STEP_FREQUENCY) % (STOP_FREQUENCY);
		if(hvFrequency < START_FREQUENCY)
			hvFrequency = START_FREQUENCY;
		setHvFlag = 1;
		_BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM1_EXIT);
	}
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
	//TA0CCR2 drives HV frequency generator
    if(P2IFG & BIT4){ //Pulse in
        P2IFG &= ~PULSE_IN;
        setCountFlag = 1;
        pulseCount++;
        windowCount++;
        soundTick();
        _BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM1_EXIT);

    }
}

//TIMER0_A0 Interrupts aren't used to toggle HV pin any more
//CCR1 used for Sound Out
#pragma vector = TIMER0_A1_VECTOR
__interrupt void T0A1_ISR(void){
	if(TA0IV == TA0IV_TACCR1){
	//CCIFG is automatically cleared
		if(!(--tickCount)){
			soundOff();
		}
    }
}

#pragma vector = TIMER1_A0_VECTOR //1-second interrupt
__interrupt void TA1CCR0_ISR(void){
	TA1CCTL0 &= ~CCIFG;
	cpm = windowCount * (60 / WINDOW_SIZE);
	windowCount = 0;
	//DO SOMETHING HERE!
}
