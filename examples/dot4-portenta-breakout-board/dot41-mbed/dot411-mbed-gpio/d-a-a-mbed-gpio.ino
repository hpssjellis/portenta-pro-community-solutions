/*
*  a-mbed-gpio.ino
*  Lights up all breakout board GPIO pins
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
* You won't see anything useful unless you connected a resistor and LED correctly from GND to each pin one after another
* 
*
********************************************************** end ****************************************************
*
*/

#include "mbed.h"
using namespace mbed;

DigitalOut myGPIOmy0(PC_13);  
DigitalOut myGPIOmy1(PC_15);  
DigitalOut myGPIOmy2(PD_4);  
DigitalOut myGPIOmy3(PD_5);  
DigitalOut myGPIOmy4(PE_3);  
DigitalOut myGPIOmy5(PG_3);  
DigitalOut myGPIOmy6(PG_10);   

void setup() {

}

void loop() {

     myGPIOmy0.write(1); // external LED 1 = on  
     myGPIOmy1.write(1);  
     myGPIOmy2.write(1);  
     myGPIOmy3.write(1); 
     myGPIOmy4.write(1);  
     myGPIOmy5.write(1); 
     myGPIOmy6.write(1);     
     wait_us(500000);    // microseconds

     myGPIOmy0.write(0); // external LED 0 = off 
     myGPIOmy1.write(0);  
     myGPIOmy2.write(0);  
     myGPIOmy3.write(0); 
     myGPIOmy4.write(0);
     myGPIOmy5.write(0); 
     myGPIOmy6.write(0);     
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
