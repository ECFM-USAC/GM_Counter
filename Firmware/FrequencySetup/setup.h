#include <msp430g2553.h>
#include "ports.h"

volatile unsigned long MCLK_freq;
volatile unsigned long SMCLK_freq;
volatile unsigned long ACLK_freq;

#define SMCLK_PRESCALER 8
#define ACLK_XTAL 32768

#ifndef SETUP_H_
#define SETUP_H_

void ioSetup(void);
void clkSetup(unsigned int freq);
void timerSetup(void);
void interruptSetup(void);
void adcSetup(void);




#endif /* SETUP_H_ */
