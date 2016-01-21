/*
 * adc.h
 *
 *  Created on: 21/01/2016
 *      Author: Ivan
 */

#define ADC10_RESOLUTION 10
#define START_FREQUENCY 2000
#define FREQUENCY_STEP 100
#define RESOLUTION_SHIFT 4 //ADC Data Right Shift (decrease resolution)
//4 Right Shifts = 6 bits = (0 -> 63) Range

#define MAX_VALUE (1 << (ADC10_RESOLUTION - RESOLUTION_SHIFT)) - 1 //63

#ifndef ADC_H_
#define ADC_H_

unsigned int frequencyFromAdc(unsigned int adcValue);

#endif /* ADC_H_ */
