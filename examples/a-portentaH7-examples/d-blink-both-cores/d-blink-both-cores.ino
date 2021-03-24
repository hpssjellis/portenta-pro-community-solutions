/*
  d-blink-both-cores.ino for the Portenta H7
  This code can be loaded on either core.
  The M7 core will initialize the M4 core
  M7 will flash Blue randomly (under 6 seconds)
  The M4 will flash Green randomly (under 6 seconds)
  July 22nd, 2020
  by Jeremy Ellis
  Twitter @rocksetta
  Website https://www.rocksetta.com
*/




int myLED; 

void setup() {
   randomSeed(analogRead(A0));
  #ifdef CORE_CM7  
     LL_RCC_ForceCM4Boot();  
     myLED = LEDB; // on-board blue LED
  #endif

  #ifdef CORE_CM4  
     myLED = LEDG; // on-board greeen LED
  #endif   
   
  pinMode(myLED, OUTPUT);
}


void loop() {
   digitalWrite(myLED, LOW); // turn the LED on (LOW is the voltage level)
   delay(200); // wait for a second
   digitalWrite(myLED, HIGH); // turn the LED off by making the voltage HIGH
   delay( rand() % 5000 + 1000); // wait for a random amount of time below 6 seconds.
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

