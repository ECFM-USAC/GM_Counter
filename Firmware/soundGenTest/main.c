#include <msp430g2553.h>

const unsigned int MAX_TICKS = 10;

volatile unsigned int ticks;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    //Set MCLK to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;
	DCOCTL  = CALDCO_16MHZ;

	//SMCLK prescaler 1:8 source from MCLK
	BCSCTL2 |= 0x06; //Set SMCLK to 2MHz


	/*
	 * P1.2 HV Freq Out
	 * P1.3 Push button input
	 * P1.6 Sound out
	 */
	P1DIR  =  0x44;
	P1OUT  =  0x00;
	P1OUT |=  0x08; //Pull-up for push-button input
	P1REN |=  0x08; //Pull-up resistor enabled for push-button input
	P1SEL |=  0x04; //TA0CCR1 output

	//TIMER0_A0 Configuration
	TA0CTL   = TASSEL_2 + MC_0 + TACLR; //SMCLK as clock source (2MHz) + OFF mode + Clear TA0R counter
	TA0CCR0  = 1000 - 1; //Frequency output set to 2kHz (2MHz / 1000 = 2kHz)
	TA0CCR1  = TACCR0 / 2; //50% duty cycle
	TA0CCTL1 = CM_0 + OUTMOD_7; //Compare mode + Output Reset/Set
	TA0CTL  |= MC_1; //Up mode

	//Push-button interrupt configuration
	P1IFG  =  0x00; //Clear all interrupt flags
	P1IES |=  0x08; //Descending edge
	P1IE  |=  0x08; //Push-button
	_BIS_SR(GIE); //General Interrupt Enable bit set in Status Register (SR)
	_BIS_SR(LPM1);

	while(1){
		;
	}

	return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void){
	if(P1IFG & BIT3){ //If push-button was pressed
		P1IFG    &= ~0x08;  //Clear interrupt flag
		P1SEL    |=  0x40;  //Enable PWM output for SoundOut
		TA0CCTL1 &= ~CCIFG; //Clear TA0CCR1 interrupt flag
		TA0CCTL1 |=  CCIE;  //Enable TA0CCR1 interrupt
		ticks = MAX_TICKS;
	}
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void T0A1_ISR(void){
	if(TA0IV == TA0IV_TACCR1){
		if(!(--ticks)){
			P1SEL    &= ~0x40; //Disable PWM for SoundOut
			TA0CCTL1 &= ~CCIE; //Disable TACCR1 Interrupts
		}
	}
}
