#include "setup.h"

#ifndef GMCORE_H_
#define GMCORE_H_

#define SOUND_FREQUENCY 2000
#define CYCLES_PER_TICK 4 //Sound cycles for each tick

#define WINDOW_SIZE 5 //Moving-average time window size (in seconds)

#define BACKLIGHT_SECONDS 5 //Backlight ON time before being turned OFF

volatile unsigned long pulseCount; //Total pulses since last reset
volatile unsigned long tickCount;  //Pulses for tick sound generation
volatile unsigned int  windowCount; //Pulses within moving-average window
volatile unsigned int  cps[60/WINDOW_SIZE]; //Array of counts per second
volatile unsigned long cpm; //Counts per minute
volatile char backlightSeconds;
char backlightEnabled;


void setHVFrequency(unsigned int freq);
void initGM(void);
void soundOn(void);
void soundOff(void);
void soundTick(void);
void resetCounters(void);
void lcdLedOn(void);
void lcdLedOff(void);

#endif /* GMCORE_H_ */
