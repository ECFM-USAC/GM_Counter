#include <msp430g2553.h>
#include "ports.h"

unsigned long MCLK_freq;
unsigned long SMCLK_freq;

#define SMCLK_PRESCALER 8

#ifndef SETUP_H_
#define SETUP_H_

void ioSetup(void);
void clkSetup(unsigned int freq);
void timerSetup(void);
void interruptSetup(void);




#endif /* SETUP_H_ */
