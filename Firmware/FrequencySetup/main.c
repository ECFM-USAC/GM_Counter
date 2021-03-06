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

	setHVFrequency(DEFAULT_FREQUENCY);

	P1DIR |= BIT7;

	while(1){
		/*
		if(setHvFlag){
			setHVFrequency(hvFrequency);
			lcd_clear();
			lcd_print("f = ");
			lcd_print_number(hvFrequency);
			lcd_print(" Hz");
			setHvFlag = 0;
			setCountFlag = 1;
		}
		*/
		//hvFrequency = 2000;
		if(setCountFlag){
			lcd_gotoRow(1);
			lcd_print("Counts: ");
			lcd_print_number(pulseCount);
			lcd_print("     ");
			lcd_gotoRow(2);
			//lcd_print("Counts: ");
			//lcd_print_number(pulseCount);
			lcd_print("CPM: ");
			lcd_print_number(cpm);
			lcd_print("        ");
			setCountFlag = 0;
		}
		_BIS_SR(LPM1);
	}
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
	if(P1IFG & BIT3){ //PUSH BUTTON
		P1IFG &= ~PUSH_BTN;
		resetCounters();
		setCountFlag = 1;
		/*
		P1IFG &= ~PUSH_BTN;
		hvFrequency = (hvFrequency + STEP_FREQUENCY) % (STOP_FREQUENCY);
		if(hvFrequency < START_FREQUENCY)
			hvFrequency = START_FREQUENCY;
		setHvFlag = 1;
		*/
		_BIS_SR(GIE); //Enable interrupts before going back to sleep
		_BIC_SR(LPM1_EXIT);
	}else if(P1IFG & BIT1){ //USER BUTTON
		P1IFG &= ~USER_BTN;
		lcdLedOn();
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

#pragma vector = TIMER1_A0_VECTOR //Interrupt every WINDOW_SIZE seconds
__interrupt void TA1CCR0_ISR(void){
	unsigned int j;
	TA1CCTL0 &= ~CCIFG; //Clear IRQ flag
	cpm = 0; //Clear cpm counter
	for(j = 1; j < (60/WINDOW_SIZE); j++){ //Shift and add cps windowed values
		cps[j] = cps[j-1];
		cpm += cps[j];
	}
	cps[0] = windowCount;
	windowCount = 0; //Clear current window counter
	cpm += cps[0];
	if(!((--backlightSeconds + 1)/WINDOW_SIZE) & backlightEnabled){
		backlightSeconds = 0;
		lcdLedOff();
	}
	setCountFlag = 1;
}
