/* 

Portenta Serial UART
// Need to connect pin TX D14 and RX D13 together.
// Passing data between the cores both using UART1 works for simple transfers but does not work for 
// complex transfers, which need UART0 and UART1 which needs the breakout board.

UART myUART0(PA_0,  PI_9,  NC, NC); //TX, RX, RTS, CTS  NOTE: NC means not connected
UART myUART1(PA_9,  PA_10, NC, NC);
UART myUART2(PG_14, PG_9,  NC, NC);
UART myUART3(PJ_8,  PJ_9,  NC, NC);


   //  or
   // Serial
   // Serial0
   // Serial1   is myUART1
   // Serial2
   // Serial3

*/
#include <Arduino.h>

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

UART myUART1(PA_9,  PA_10, NC, NC);


void setup() { 
   bootM4();    
   Serial.begin(115200);
   myUART1.begin(115200);   
}

void loop() {
  if (myUART1.available()) {          // If anything comes in Serial3 
     Serial.write(myUART1.read());    // Read it and send it out Serial (USB)
  }




}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

int  myCount=48;  //48 = ascii 0,    58 ascii 9

UART myUART1(PA_9,  PA_10, NC, NC);



void setup() { 
    pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   // Serial.begin(115200);  // no serial monitor on M4 core without RPC
    myUART1.begin(115200);   

}

void loop() {
    myCount++;
   if (myCount >= 58){myCount = 48;}           // ascii 48 =  number 0,   ascii 58 = number 9
   digitalWrite(LEDB, !digitalRead(LEDB));     // switch on / off
   char x = (char)myCount;  //random(48, 57);  // ascii code for numbers 0 to 9
   myUART1.write(x); 
   //delayMicroseconds(90);                    // unsigned int max value 65535
   delay(700); 
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
