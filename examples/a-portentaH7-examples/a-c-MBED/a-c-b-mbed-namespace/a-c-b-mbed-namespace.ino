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
*/



#include "mbed.h"
using namespace mbed;

DigitalOut myLedRed(PK_5); // red
DigitalOut myLedGreen(PK_6); // green
DigitalOut myLedBlue(PK_7); // blue

void setup() {
}

void loop() {
  myLedRed = 0;   // red on
  myLedGreen = 0;   // green on
  myLedBlue = 0;   // blue on
  wait_us(1000000);  // 1 million = 1 second delay
  
  myLedRed = 1;  // off
  myLedGreen = 1;
  myLedBlue = 1;
  wait_us(2000000);
}






/*
* Clickable links for helpful information
* By @rocksetta
* March, 2021
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
*
*
*
*  Arduino Pro Links:
*
*  https://store.arduino.cc/usa/portenta-h7
*  https://forum.arduino.cc/index.php?board=148.0
*  https://www.arduino.cc/pro/tutorials/portenta-h7
*
*  Rocksetta links:
* 
*  https://twitter.com/rocksetta
*  https://github.com/hpssjellis/portenta-pro-community-solutions
*  https://github.com/hpssjellis/my-examples-for-the-arduino-portentaH7
*  https://github.com/hpssjellis/arduino-high-school-robotics-course
*  https://www.youtube.com/playlist?list=PL57Dnr1H_egtm0pi-okmG0iE_X5dROaLw
*
*
*
*/



