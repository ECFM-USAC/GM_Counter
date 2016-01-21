#include <msp430.h> 

unsigned int adcVal;
unsigned int avg;
unsigned long cnt;
unsigned int freqOut;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
    BCSCTL1 |= XT2OFF + DIVA_3; //XT2 OFF + 1:8 PRESCALER FOR ACLK
	BCSCTL3 |= LFXT1S_0 + XCAP_3;

    TA1CTL    = TASSEL_1 + ID_3 + MC_1 + TACLR; //ACLK + 1:8 PRESCALER + UP MODE + TA1 CLEAR
	TA1CCR0   = 1*512 - 1; //IRQ every 1 seconds
	
	TA1CCTL0 &= ~CCIFG; //Clear TimerA1_CCR0 Interrupt Flag
	TA1CCTL0  = CM_0 + CCIE; //Enable interrupts for TA1CCR0

    P2OUT = 0x00;
    P2DIR = 0xFF;

    P3OUT = 0x00;
	P3DIR = 0xFF;

    P1OUT = 0x00;
    P1DIR = 0x01;

    ADC10CTL0 = 0x0E80; //Reference buffer enabled only during conversion. ADC OFF. Slow sampling rate.

    ADC10CTL1 = INCH_7 | ADC10DIV_7 | ADC10SSEL_3; //ADC Ch 7 input single-sample mode. 1/8 CLK Divider. SMCLK as source.
    ADC10AE0 =  0x80; //Analog Input Ch 7 multiplexer enabled

    //Direct memory transfer control disabled
    ADC10DTC0 = 0x00;
    ADC10DTC1 = 0x00;


    ADC10CTL0 |= ADC10IE; //ADC Interrupts Enabled
    ADC10CTL0 |= ADC10SC | ENC | ADC10ON; //Start first conversion

    cnt = 0;
    avg = 0;

    _BIS_SR(GIE); //General interrupts enable

    _BIS_SR(LPM1);
   /* while(1){
    	;
    }*/
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void T1ACCR0_ISR(void){
	ADC10CTL0 |= ADC10SC | ENC | ADC10ON; //Enable ADC Sampling
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void){
	ADC10CTL0 &= ~ADC10IFG;
	P1OUT ^= 0x01; //Toggle Red LED
	adcVal = ADC10MEM; //Fetch ADC Data
	avg = (avg + adcVal) >> 1;
	freqOut = 2000 + (63 - (adcVal >> 4)) * 100;
	++cnt;
}