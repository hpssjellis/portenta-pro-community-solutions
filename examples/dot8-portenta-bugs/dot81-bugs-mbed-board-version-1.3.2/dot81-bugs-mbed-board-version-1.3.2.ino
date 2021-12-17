/*
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
* 
*
********************************************************** end ****************************************************
*
Errors I have been finding for Version 1.3.2 mbed core on the Portenta 
Tested on the M7 core

These are all using the 
#include "mbed.h";

Analog Pins:

AnalogIn   myA3(PC_3C);    // does this work?
AnalogIn   myA4(PC_2);     // is this actually A5
AnalogIn   myA5(PC_3);     // Does this work?


PWM Pins:

PwmOut  myPWM3(PG_7);  // broken
PwmOut  myPWM4(PJ_11);  // broken






