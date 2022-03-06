/*  For any gotchas that might not be obvious to a new user
*
*  Example: A0, A1, A2, A3 not to be used for general GPIO 
*  All Analog pins are not to be used for PWM
*
*
*
*
*
*
*
*

Not a problem with Arduino but if using edgeimpulse then be aware of these re-defines in the main cpp program loaded

SHould mean pin numbers 0, 1, 2 are fine but A0, A1, A2 are not fine to use.


// Undefine min/max macros as these conflict with C++ std min/max functions
// these are often included by Arduino cores
#include <Arduino.h>
#include <stdarg.h>
#ifdef min
#undef min
#endif // min
#ifdef max
#undef max
#endif // max
#ifdef round
#undef round
#endif // round
// Similar the ESP32 seems to define this, which is also used as an enum value in TFLite
#ifdef DEFAULT
#undef DEFAULT
#endif // DEFAULT
// Infineon core defines this, conflicts with CMSIS/DSP/Include/dsp/controller_functions.h
#ifdef A0
#undef A0
#endif // A0
#ifdef A1
#undef A1
#endif // A1
#ifdef A2
#undef A2
#endif // A2


*
*
*
*
*
*
*
*
*
*
*
*/


