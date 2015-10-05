//----------- PIN DEFINITIONS --------------

#ifndef PORTS_H_
#define PORTS_H_

//--------------- PORT 1 ---------------
#define LED_OK    (0x0001); //P1.0
#define FREQ_OUT  (0x0004); //P1.2
#define PUSH_BTN  (0x0008); //P1.3
#define SOUND_OUT (0x0040); //P1.6

//--------------- PORT 2 ---------------
#define PULSE_IN  (0x0010); //P2.4

//--------------- LCD ------------------
//At least, LCD_Dx bits must be in the same port (either P2 or P1)

#define LCD_RS	(0x0010); //P1.4
#define LCD_EN	(0x0020); //P1.5
#define LCD_D4	(0x0001); //P2.0
#define LCD_D5	(0x0002); //P2.1
#define LCD_D6	(0x0004); //P2.2
#define LCD_D7	(0x0008); //P2.3

#endif /* PORTS_H_ */
