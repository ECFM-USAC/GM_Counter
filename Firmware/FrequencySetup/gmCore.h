#include "setup.h"

#ifndef GMCORE_H_
#define GMCORE_H_

#define SOUND_FREQUENCY 2000
#define CYCLES_PER_TICK 4 //Sound cycles for each tick

volatile unsigned long pulseCount;
volatile unsigned long tickCount;


void setHVFrequency(unsigned int freq);
void initGM(void);
void soundOn(void);
void soundOff(void);
void soundTick(void);

#endif /* GMCORE_H_ */
