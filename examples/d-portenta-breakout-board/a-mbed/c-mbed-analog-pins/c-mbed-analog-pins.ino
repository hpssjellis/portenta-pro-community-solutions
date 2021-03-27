/*
* c-mbed-analog-pins.ino
* reads each analog pin every 3 seconds and prints to serial
*
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
* Runs without anything connected, but you could test each one
* with a resistor from 3V3 to each Analog pin
*
********************************************************** end ****************************************************
*
*/

#include "mbed.h"
using namespace mbed;

AnalogIn   myA0(PA_0C);
AnalogIn   myA1(PA_1C);
AnalogIn   myA2(PC_2C);
AnalogIn   myA3(PC_3C);    // does this work?
AnalogIn   myA4(PC_2);     // is this actually A5
AnalogIn   myA5(PC_3);     // Does this work?
AnalogIn   myA6(PA_4);
AnalogIn   myA7(PA_6);
   

void setup() {
   Serial.begin(115200);

}

void loop() {
   Serial.println("Reading Analog pin A0: " + String(myA0.read(), 3) );
   Serial.println("Reading Analog pin A1: " + String(myA1.read(), 3) );
   Serial.println("Reading Analog pin A2: " + String(myA2.read(), 3) );
   Serial.println("Reading Analog pin A3: " + String(myA3.read(), 3) );
   Serial.println("Reading Analog pin A4: " + String(myA4.read(), 3) );
   Serial.println("Reading Analog pin A5: " + String(myA5.read(), 3) );
   Serial.println("Reading Analog pin A6: " + String(myA6.read(), 3) );
   Serial.println("Reading Analog pin A7: " + String(myA7.read(), 3) );
   Serial.println("----------------------------------");
      
     
   wait_us(3000000);    // microseconds : 3 seconds
 

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
