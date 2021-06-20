// Easy Dual Core program setup

// Need to connect pin TX D14 and RX D13 together.
// USing Serial1 on both cores only works for sending simple data.
// For complex data we need the breakout board, examples for ArduinoJSON and SerialTransfer there.


#include <Arduino.h>

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

void setup() { 
   bootM4();    
   Serial.begin(115200);
   Serial1.begin(9600);   
}

void loop() {
  if (Serial1.available()) {          // If anything comes in Serial3 
     Serial.write(Serial1.read());    // Read it and send it out Serial (USB)
  }

}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

int  myCount=48;  //48 = ascii 0,    58 ascii 9

void setup() { 
    pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   // Serial.begin(115200);
    Serial1.begin(9600);   

}

void loop() {
    myCount++;
   if (myCount >= 58){myCount = 48;}         //48 = ascii 0,    58 ascii 9
   digitalWrite(LEDB, !digitalRead(LEDB));   //switch on / off
   char x = (char)myCount;                   //random(48, 57);  // ascii code for numbers 0 to 9
   Serial1.write(x); 
   delayMicroseconds(2000000);                     // start at 100000 and check the order. mine messes up at 2 microseconds
  
    
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
