#include "setup.h"
#include "gmCore.h"


void ioSetup(void){
    P1DIR  =  0x00;
    P1DIR |=  LED_OK;
    P1DIR |=  FREQ_OUT;
    P1DIR &= ~USER_BTN; //User button
    P1DIR &= ~PUSH_BTN; //Reset counter button
    P1DIR |=  SOUND_OUT; //Sound out otput
    P1REN  =  PUSH_BTN; //Enable pull-U/D restitor capabilities on PUSH_BTN
    P1REN |=  USER_BTN; //Enable pull-U/D restitor capabilities on USER_BTN
    P1SEL |=  FREQ_OUT; //PWM Output for HV Frequency Generator
    P1SEL2 =  0x00;

    P2DIR  =  0x00;
    P2DIR &= ~PULSE_IN; //Input pulses
    P2REN |=  PULSE_IN; //Enable pull-down resistor for PULSE_IN
    P2SEL |= (BIT6 | BIT7); //ACLK input for 32768 Hz XTAL


    //LCD pins
    //P1DIR |= (LCD_RS + LCD_EN);
    //P2DIR |= (LCD_D4 + LCD_D5 + LCD_D6 + LCD_D7);
    P1DIR |=  LCD_RS;
    P1DIR |=  LCD_EN;
    P2DIR |=  LCD_D4;
    P2DIR |=  LCD_D5;
    P2DIR |=  LCD_D6;
    P2DIR |=  LCD_D7;
    P2DIR |=  LCD_LED;

    //Set POR values
    P1OUT  =  0x00;
    P1OUT |=  PUSH_BTN; //Pull-up
    P1OUT |=  USER_BTN; //Pull-up
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
    BCSCTL1 |= XT2OFF + DIVA_3; //XT2 OFF + 1:8 PRESCALER FOR ACLK
    BCSCTL3 |= LFXT1S_0 + XCAP_3;

    MCLK_freq  = freq * 1000000;
    SMCLK_freq = MCLK_freq / SMCLK_PRESCALER;
    ACLK_freq  = ACLK_XTAL / 8;

}


void timerSetup(void){
    TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR; //SMCLK + 1:1 PRE + UP MODE + CLEAR TMR
    TA0CCTL1 = OUTMOD_7;                     //RESET/SET MODE FOR PWM

    TA1CTL    = TASSEL_1 + ID_3 + MC_1 + TACLR; //ACLK + 1:8 PRESCALER + UP MODE + TA1 CLEAR
    TA1CCR0   = WINDOW_SIZE*512 - 1; //IRQ every WINDOW_SIZE seconds

}


void interruptSetup(void){
    P1IES |=  PUSH_BTN; //Descending edge for interrupt detection on PUSH_BTN
    P1IES |=  USER_BTN; //Descending edge for interrupt detection on USER_BTN
    P1IFG  =  0x00; //Clear all interrupt flags from P1
    P1IFG &= ~PUSH_BTN; //Clear PUSH_BTN interrupt flag before enabling it
    P1IFG &= ~USER_BTN;
    P1IE   =   0x00; //Disable all interrupts on P1
    P1IE  |=  PUSH_BTN; //Enable PUSH_BTN interrupt
    P1IE  |=  USER_BTN;

    P2IFG  =  0x00; //Clear all interrupt flags from P2
    P2IE   =  0x00; //Disable all interrupts on P2
    P2IES  =  0x00; //Ascending edge interrupt for P2
    P2IE   =  0x00; //Disable all interrupts on P2
    P2IE  |=  PULSE_IN; //Enable interrupts for GM counts

    TA1CCTL0 &= ~CCIFG; //Clear TimerA1_CCR0 Interrupt Flag
    TA1CCTL0  = CM_0 + CCIE; //Enable interrupts for TA1CCR0

    _BIS_SR(GIE); //General interrupts enable
}
