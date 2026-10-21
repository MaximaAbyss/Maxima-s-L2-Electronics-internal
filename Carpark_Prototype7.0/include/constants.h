#ifndef constants
#define constants

#include <Arduino.h>

extern bool gate1running;
extern bool gate2running;

extern const int MUX_A;
extern const int MUX_B;
extern const int MUX_C;
extern const int SR_DATAPIN;
extern const int SR_CLOCKPIN;

extern bool ledOutput[8];
extern int cars_in_lot;
extern int cars_parked;

#endif