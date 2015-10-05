#include "setup.h"
#include "gmCore.h"


void ioSetup(void){
    P1DIR  =  0x00;
    P1DIR |=  LED_OK;
    P1DIR |=  FREQ_OUT;
    P1DIR &= ~PUSH_BTN; //INPUT
    P1DIR |=  SOUND_OUT; //Sound out otput
    P1REN  =  PUSH_BTN; //Enable pull-U/D restitor capabilities on PUSH_BTN
    P1SEL |=  FREQ_OUT; //PWM Output for HV Frequency Generator
    P1SEL2 =  0x00;

    P2DIR  =  0x00;
    P2DIR &= ~PULSE_IN; //Input pulses
    P2REN |=  PULSE_IN; //Enable pull-down resistor for PULSE_IN


    //LCD pins
    //P1DIR |= (LCD_RS + LCD_EN);
    //P2DIR |= (LCD_D4 + LCD_D5 + LCD_D6 + LCD_D7);
    P1DIR |=  LCD_RS;
    P1DIR |=  LCD_EN;
    P2DIR |=  LCD_D4;
    P2DIR |=  LCD_D5;
    P2DIR |=  LCD_D6;
    P2DIR |=  LCD_D7;

    //Set POR values
    P1OUT  =  0x00;
    P1OUT |=  PUSH_BTN; //Pull-up
    P2OUT  =  0x00;

    initGM(); //Initialize GM Core
}


void clkSetup(unsigned int freq){  //Frequency given in MHz
    switch(freq){
        case 1:
            BCSCTL1 = CALBC1_1MHZ;
            DCOCTL  = CALDCO_1MHZ;
            break;

        case 8:
            BCSCTL1 = CALBC1_8MHZ;
            DCOCTL  = CALDCO_8MHZ;
            break;

        case 12:
            BCSCTL1 = CALBC1_12MHZ;
            DCOCTL  = CALDCO_12MHZ;
            break;

        case 16:
            BCSCTL1 = CALBC1_16MHZ;
            DCOCTL  = CALDCO_16MHZ;
            break;

        default:
            BCSCTL1 = CALBC1_1MHZ;
            DCOCTL  = CALDCO_1MHZ;
            freq    = 1;
    }
    BCSCTL2 |= 0x06; //1:8 Prescaler for SMCLK
    MCLK_freq = freq * 1000000;
    SMCLK_freq = MCLK_freq / SMCLK_PRESCALER;
}


void timerSetup(void){
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR; //SMCLK + 1:1 PRE + UP MODE + CLEAR TMR
    TA0CCTL1 = OUTMOD_7;                     //RESET/SET MODE FOR PWM

    /*//TA1 Not used to generate sound any more
    TA1CTL = TASSEL_2 + ID_0 + MC_0 + TACLR; //SMCLK + 1:1 PRE + STOP + CLEAR TMR
    TA1CCTL2  = OUTMOD_7;                    //RESET/SET MODE FOR PWM
    */
}


void interruptSetup(void){
    P1IES |=  PUSH_BTN; //Descending edge for interrupt detection on PUSH_BTN
    P1IFG  =  0x00; //Clear all interrupt flags from P1
    P1IFG &= ~PUSH_BTN; //Clear PUSH_BTN interrupt flag before enabling it
    P1IE  =   0x00; //Disable all interrupts on P1
    P1IE  |=  PUSH_BTN; //Enable PUSH_BTN interrupt

    P2IFG  =  0x00; //Clear all interrupt flags from P2
    P2IE   =  0x00; //Disable all interrupts on P2
    P2IES  =  0x00; //Ascending edge interrupt for P2
    P2IE   =  0x00; //Disable all interrupts on P2
    P2IE  |=  PULSE_IN; //Enable interrupts for GM counts

    //TACCTL0 = CCIE; //TACCR0 Interrupt Enable //----- TACCR0 Interrupts ain't used any more
    _BIS_SR(GIE); //General interrupts enable
}
