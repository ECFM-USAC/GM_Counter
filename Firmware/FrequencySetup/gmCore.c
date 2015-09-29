#include "gmCore.h"
#include "timing.h"
// ------ Set frequency for HV operation -------------
void setHVFrequency(unsigned int freq){ //Frequency given in Hz
    TACCR0 = getTACCRfromFreq(freq);
    TACCR1 = TACCR0 / 2; // 50% Duty Cycle
}

void setSoundFrequency(unsigned int freq){
    TA1CCR0 = getTACCRfromFreq(freq);
    TA1CCR2 = TA1CCR0 / 2;
}

void initGM(void){
    pulseCount = 0;
    setSoundFrequency(2000);
}

void soundOn(void){
    TA1CTL   |= MC_1 + TACLR;  //TA1 Mode: UP
    TA1CCTL2 |= CCIE; //Enable interrupts
}

void soundOff(void){
    TA1CTL   &= ~0x30; //TA1 Mode: OFF
    TA1CTL   |= TACLR; //Clear TIMER1_A counter
    TA1CCTL2 &= ~CCIE; //Disable interrupts
}

void soundTick(void){ //Make a "tick" GM-like sound
    tickCount = 0; //How many cycles per tick?
    soundOn(); //Sound will be turned off within ISR
}
