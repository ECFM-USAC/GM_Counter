#include "gmCore.h"
#include "timing.h"
#include "ports.h"
// ------ Set frequency for HV operation -------------
void setHVFrequency(unsigned int freq){ //Frequency given in Hz
    TACCR0 = getTACCRfromFreq(freq);
    TACCR1 = TACCR0 >> 1; // 50% Duty Cycle
}


void initGM(void){
	unsigned char j;
	pulseCount  = 0;
	for(j = (60/WINDOW_SIZE); j > 0; j--){
    	cps[j] = 0;
    }

	lcdLedOn();

}

void soundOn(void){
    P1SEL    |=  SOUND_OUT; //Enable PWM output for SoundOut
	TA0CCTL1 &= ~CCIFG; //Clear TA0CCR1 interrupt flag
	TA0CCTL1 |=  CCIE;  //Enable TA0CCR1 interrupt
}

void soundOff(void){
	P1SEL    &= ~SOUND_OUT; //Disable PWM output mux for SoundOut
	TA0CCTL1 &= ~CCIE; //Disable TACCR1 Interrupts
}

void cntLedOn(void){
	P1OUT |= LED_OK;
}

void cntLedOff(void){
	P1OUT &= ~LED_OK;
}

void soundTick(void){ //Make a "tick" GM-like sound
	tickCount = CYCLES_PER_TICK; //How many cycles per tick?
    soundOn(); //Sound will be turned off within ISR
}

void ledTick(void){
	cntLedOn();
}

void resetCounters(void){ //Reset CPM, CPS and Counts
	char interruptEnabled = 0;
	if(P2IE & BIT4){
		interruptEnabled = 1;
		P2IE &= ~PULSE_IN;
	}
	unsigned int k;
	pulseCount = 0;
	windowCount = 0;
	cpm = 0;
	for(k = 60/WINDOW_SIZE; k > 0; k--){
		cps[k] = 0;
	}
	if(interruptEnabled){
		P2IE |= PULSE_IN;
	}
}

void lcdLedOn(void){
	backlightSeconds = BACKLIGHT_SECONDS;
	backlightEnabled = 1;
	P2OUT |=  LCD_LED;
}

void lcdLedOff(void){
	backlightEnabled = 0;
	P2OUT &= ~LCD_LED;
}
