#include <msp430g2553.h>

unsigned int i = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	P1DIR  =  0x00;
	P1DIR |=  0x01; //P1.0 LED
	P1OUT  =  0x00;
	P2SEL |= (BIT6 | BIT7); //ACLK input


	BCSCTL1 = XT2OFF + DIVA_3; //XT2 OFF + 1:8 PRESCALER FOR ACLK
	BCSCTL3 = LFXT1S_0 + XCAP_3;

	TA1CTL    = TASSEL_1 + ID_3 + MC_1 + TACLR; //ACLK + 1:8 PRESCALER + UP MODE + TA1 CLEAR
	TA1CCR0    = 256 - 1; //1 interrupt every second
	//TA1CCR0    = 4096 - 1; //1 interrupt every 8 seconds
	TA1CCTL0 &= ~CCIFG;
	TA1CCTL0  = CM_0 + CCIE;
	_BIS_SR(GIE);

	//_BIS_SR(LPM2);

	while(1){
		;
	}

	return 0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TA1CCR0_ISR(void){
	TA1CCTL0 &= ~CCIFG;
	P1OUT ^= 0x01; //Toggle LED
	i++;
}
