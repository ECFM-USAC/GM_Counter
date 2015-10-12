#include "setup.h"

#ifndef GMCORE_H_
#define GMCORE_H_

#define SOUND_FREQUENCY 2000
#define CYCLES_PER_TICK 4 //Sound cycles for each tick

#define WINDOW_SIZE 5 //Moving-average time window size (in seconds)

volatile unsigned long pulseCount; //Total pulses since last reset
volatile unsigned long tickCount;  //Pulses for tick sound generation
volatile unsigned int  windowCount; //Pulses within moving-average window
volatile unsigned int  cps[60/WINDOW_SIZE]; //Array of counts per second
volatile unsigned long cpm; //Counts per minute


void setHVFrequency(unsigned int freq);
void initGM(void);
void soundOn(void);
void soundOff(void);
void soundTick(void);

#endif /* GMCORE_H_ */
