#include "adc.h"

unsigned int frequencyFromAdc(unsigned int adcValue){
	adcValue >>= RESOLUTION_SHIFT;
	return (START_FREQUENCY) + (MAX_VALUE - adcValue) * FREQUENCY_STEP;
	//return (START_FREQUENCY) + (adcValue) * FREQUENCY_STEP;
}