#include "mbed.h"
using namespace mbed;

DigitalOut LD1(PK_5); // red
DigitalOut LD2(PK_6); // green
DigitalOut LD3(PK_7); // blue

void setup() {
}

void loop() {
  LD1 = 0;   // red on
  LD2 = 0;   // green on
  LD3 = 0;   // blue on
  wait_us(1000000);  // 1 million = 1 second delay
  
  LD1 = 1;  // off
  LD2 = 1;
  LD3 = 1;
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
*  Artduino Pro Links:
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



