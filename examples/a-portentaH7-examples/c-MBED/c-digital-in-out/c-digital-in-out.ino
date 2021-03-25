

#include "mbed.h"
using namespace mbed;

DigitalInOut myPin(LED1);  // Any pin

void setup() {
    Serial.begin(115200);
}

void loop() {
    // check that mypin object is initialized and connected to a pin
    if (myPin.is_connected()) {
        Serial.println("myPin is initialized and connected!");
       // printf("myPin is initialized and connected!\n\r");
    }

    // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
    myPin.mode(PullNone);

    while (1) {
        // write to pin as output
        myPin.output();
        myPin = !myPin; // toggle output
        wait_us(500000);

        // read from pin as input
        myPin.input();
       // printf("mypin.read() = %d \n\r", myPin.read());
        Serial.println("mypin.read() =" + String(myPin.read())  );

        wait_us(500000);
    }
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
