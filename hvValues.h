#define START_FREQUENCY 1000
#define STOP_FREQUENCY 6000
#define STEP = 100
#define COUNT = (START_FREQUENCY - STOP_FREQUENCY) / STEP + 1

const unsigned int VOLTAGES[COUNT] = {
	524, 524, 524, 524, 524,
	524, 524, 524, 524, 524,
	524, 524, 520, 520, 520,
	512, 496, 484, 476, 468,
	464, 452, 448, 436, 432,
	420, 420, 408, 400, 400,
	392, 380, 376, 376, 372,
	360, 356, 348, 348, 344,
	340, 332, 324, 316, 316,
	316, 316, 312, 308, 304,
	300
}

const unsigned int ORIGINAL_VOLTAGES[COUNT] = {
	524, 524, 524, 524, 524,
	524, 524, 524, 524, 524,
	524, 524, 520, 520, 520,
	512, 496, 484, 476, 468,
	464, 452, 448, 436, 432,
	420, 420, 408, 400, 400,
	392, 380, 376, 376, 372,
	360, 356, 348, 352, 344,
	340, 332, 324, 316, 316,
	324, 316, 312, 308, 304,
	300
}