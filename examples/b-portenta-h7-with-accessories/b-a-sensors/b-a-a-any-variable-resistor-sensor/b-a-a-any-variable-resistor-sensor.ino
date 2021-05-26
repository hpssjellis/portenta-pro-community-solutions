/*  
 * a-any-variable-resistor-sensor.ino  (Voltage Divider)
 *
 * GNU GENERAL PUBLIC LICENSE
 * Use at your own risk.
 ************************************************ Important stuff if needed ****************************************
 *
 *  Your need a sensor that is a variable resistor. Most sensors that just have 2 wires coming from them are variable resistors.
 *
 ********************************************************** end ****************************************************
 *
 *
 * GND to resistor (try different resistors I used a 5 k ohm, larger resistor larger range of values)
 * resistor to both A0 and a 2 prong sensor (such as a photoresistor)
 * other side of 2 prong sensor to 3V3
 *   
 *  By Jeremy Ellis twitter @rocksetta
 *  Webpage http://rocksetta.com
 *  Arduino High School Robotics Course at
 *  https://github.com/hpssjellis/arduino-high-school-robotics-course
 * 
 *  Update Feb 7th, 2020
 * 
 *  analogReadResolution(12) assumed default
 */

#include <Arduino.h>  // only needed for https://platformio.org/

void setup(){  
  
  Serial.begin(115200);
  
}


void loop() {

  Serial.println("Max = 4095, Analog Read A0: " + String(analogRead(A0)) );

  delay(3000);    // wait a bit
                  
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

