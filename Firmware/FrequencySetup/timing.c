#include "timing.h"


//------ Compute TACCR0 counter limit based on desired frequency output -------
unsigned int getTACCRfromFreq(unsigned int freq){ //Input frequency in Hz
    //As frequencies are low enough, no prescaler has been set
    unsigned long cval;
    cval = (SMCLK_freq)/(freq) - 1;
    return (cval <= 65535)?(unsigned int)cval:65535; //Ranging from 31Hz to 1MHz. :) NICE!
}
