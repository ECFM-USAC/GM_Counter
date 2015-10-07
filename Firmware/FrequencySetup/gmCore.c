#include "gmCore.h"
#include "timing.h"
#include "ports.h"
// ------ Set frequency for HV operation -------------
void setHVFrequency(unsigned int freq){ //Frequency given in Hz
    TACCR0 = getTACCRfromFreq(freq);
    TACCR1 = TACCR0 / 2; // 50% Duty Cycle
}


void initGM(void){
    pulseCount  = 0;
    windowCount = 0;
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

void soundTick(void){ //Make a "tick" GM-like sound
	tickCount = CYCLES_PER_TICK; //How many cycles per tick?
    soundOn(); //Sound will be turned off within ISR
}
