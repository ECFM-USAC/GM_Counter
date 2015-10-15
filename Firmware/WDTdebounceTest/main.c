#include <msp430g2553.h>

/*
 * main.c
 */

#define PUSH_BUTTON 0x08;
#define LED			0x01;

char pushDetected; //Was that a valid push-button trigger?
unsigned int i = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    //Set DCO to 1MHz
    BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	BCSCTL1 |= XT2OFF + DIVA_3; //XT2 OFF + 1:8 PRESCALER FOR ACLK
	BCSCTL3 |= LFXT1S_0 + XCAP_3;

	P1DIR  =  0x00;
	P1DIR |=  LED;
	P1DIR &= ~PUSH_BUTTON;
	P1OUT &= ~LED;
	P1OUT |=  PUSH_BUTTON; //Push button pull-up
	P1REN |=  PUSH_BUTTON; //Enable pull-up resistor

	//WDT: PASSWORD + HOLD + TIMER MODE + CLEAR CNT + ACLK source
	WDTCTL  = WDTPW + WDTHOLD + WDTTMSEL + WDTCNTCL + WDTSSEL;

	P1IES |=  PUSH_BUTTON; //Falling edge
	P1IE  |=  PUSH_BUTTON; //Enable interrupt for Push_button
	_BIS_SR(GIE); //General interrupt-enable bit

	while(1){
		if(pushDetected){
			pushDetected = 0;
			P1OUT  ^= LED;
		}
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void){
	if(P1IFG & BIT3){
		P1IFG  &= ~PUSH_BUTTON;
		pushDetected = 0;
		WDTCTL  = (WDTPW + WDTTMSEL + WDTCNTCL + WDTSSEL + WDTIS1 + WDTIS0); //WDT Interval Select (Clk/64) ~ 15 miliseconds
		IFG1   &= ~WDTIFG;
		IE1    |= WDTIE;
	}
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void){
		WDTCTL  = (WDTPW + WDTHOLD + WDTTMSEL + WDTCNTCL + WDTSSEL + WDTIS1 + WDTIS0);
		if(!(P1IN & BIT3)){
			pushDetected = 1;
			++i;
		}
}
