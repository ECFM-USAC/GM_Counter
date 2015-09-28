#include "gmCore.h"
#include "timing.h"
// ------ Set frequency for HV operation -------------
void setHVFrequency(unsigned int freq){ //Frequency given in Hz
    TACCR0 = getTACCRfromFreq(freq);
    TACCR1 = TACCR0 / 2; // 50% Duty Cycle
}
