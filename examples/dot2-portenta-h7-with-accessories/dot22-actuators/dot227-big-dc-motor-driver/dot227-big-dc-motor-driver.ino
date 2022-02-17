// big motor driver check
// By Jeremy Ellis
// MIT license

// for now reference at https://github.com/hpssjellis/particle.io-photon-high-school-robotics/tree/master/a11-dc-motor-drivers
// although pin names wrong

// You are suppossed to get it working using the web-app
// Draw your circuit diagram first
// This program will just tell you if  the connections are working
// See https://www.pololu.com/product/1451 for assistance

// On motor driver board LED goes red for one direction and green for the other

#include <Arduino.h> // Only needed by https://platformio.org/


void setup() {
    pinMode(D3, OUTPUT);   // digital 0 to 1
    pinMode(D4, OUTPUT);   // PWM 0 to 255
    pinMode(D5, OUTPUT);   // digital 0 to 1
    
    pinMode(LEDB, OUTPUT); // onboard LED on if LOW

}

void loop() {
    
    digitalWrite(LEDB, 0);    // D7 on

    // both off = glide, both on = brake (if motor can do that) 
    digitalWrite(D5, 1);    // set one direction
    digitalWrite(D3, 0);    // set one direction 
    
    analogWrite(D4, 50);   // go medium
    delay(3000);
    
    analogWrite(D4, 0);     // stop
    delay(3000); 
   

    digitalWrite(D5, 0);    // set other direction
    digitalWrite(D3, 1);    // set one direction
    
    analogWrite(D4, 50);   // go medium   
    delay(1000);    
    
    analogWrite(D4, 255);   // go full speed
    delay(1000);
    
    analogWrite(D4, 0);     // stop
    digitalWrite(LEDB, 1);  // D7 off
    delay(9000);            // wait 9 seconds
        
}
