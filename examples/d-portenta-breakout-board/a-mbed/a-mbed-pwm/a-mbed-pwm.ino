/*
*   a-mbed-pwm.ino
* Activates all Breakout Board PWM pins. Note: Presently PWM 3 and 4 are broken
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


DigitalOut myLedBlue(PK_7); //PK_7 =  blue, PK_6 = green , PK_5 = red

PwmOut  myPWM0(PA_8);  
PwmOut  myPWM1(PC_6);  
PwmOut  myPWM2(PC_7); 
//PwmOut  myPWM3(PG_7);  // broken

PwmOut  myPWM4(PJ_11);  // broken
PwmOut  myPWM5(PK_1); 
PwmOut  myPWM6(PH_15);  

PwmOut  myPWM7(PJ_7);  
PwmOut  myPWM8(PJ_10);  
PwmOut  myPWM9(PH_6);   

void setup() {

}

void loop() {
   myLedBlue.write(0); // internal LED 0 = 0n 
   float i;
   for (i = 0; i <= 1; i += 0.01){ 
     myPWM0.write(i);
     myPWM1.write(i);
     myPWM2.write(i);
     //myPWM3.write(i);   // broken
     myPWM4.write(i);     // compiles but does not work
     myPWM5.write(i);
     myPWM6.write(i);
     myPWM7.write(i);
     myPWM8.write(i);
     myPWM9.write(i);
     wait_us(10000);
   }
    myLedBlue.write(1); // internal LED 1 = 0ff 
    wait_us(500000);
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
