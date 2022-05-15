/* 
*  a-hello-blink.ino for the Portenta H7
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
*
*
********************************************************** end ****************************************************
*
*  Turns on the blue LED for one second, then off for three seconds, repeatedly.
*  also checks if Serial Printing is working
*  July 22nd, 2020
*  by Jeremy Ellis
*  Twitter @rocksetta
*  Website https://www.rocksetta.com
*/

#include <Arduino.h> // Only needed by https://platformio.org/

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);   // try on Portenta LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
}

void loop() {
  Serial.println("Serial print works on the Portenta M7 core only. Here is A0 reading: " + String(analogRead(A0)) );
  digitalWrite(LED_BUILTIN, LOW);   // internal LED LOW = on for onboard LED
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(3000);               
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

