/* Portenta Serial UART
*
* ================================================================================
* For this program
* 
* Connect Serial1 pin D14 TX to myUART6 pin D4 RX 
* Connect Serial1 pin D13 RX to myUART6 pin D5 TX 
* 
* ================================================================================
* 
* * UART Information for the PortentaH7 
* 
* UART myUART0(PA_0,  PI_9,  NC, NC);  // TX, RX, RTS, CTC  NOTE: NC means not connected
* UART myUART1(PA_9,  PA_10, NC, NC);   // pin D14 TX, pin D13 RX same as pre-defined Serial1
* UART myUART2(PG_14, PG_9,  NC, NC);
* UART myUART3(PJ_8,  PJ_9,  NC, NC);
* UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX
* UART myUART8(NC,    PA_8, NC, NC);    // pin D6 RX only can receive
* 
*/


#include <Arduino.h> // Only needed for https://platformio.org/

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

void setup() { 
   bootM4(); // get the M4 core running   
   Serial.begin(115200); // regular SerialUSB for printing to monitor
   
                         // Note: Crash data (RED LED) also sent along this UART channel
   Serial1.begin(9600);  // same as myUART1  pin D14 TX, pin D13 RX
}

void loop() {
  if (Serial1.available()) {          // If anything comes in Serial1 
     Serial.write(Serial1.read());    // Read it and send it out over SerialUSB to the monitor
  }

}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

int  myCount=48;  //48 = ascii 0,    58 ascii 9
UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX


void setup() { 
    pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   // Serial.begin(115200);
    myUART6.begin(9600);   

}

void loop() {
    myCount++;
   if (myCount >= 58){myCount = 48;}        // 48 = ascii 0,    58 ascii 9
   digitalWrite(LEDB, !digitalRead(LEDB));  // switch on / off
   char x = (char)myCount;                  // random(48, 57);  // ascii code for numbers 0 to 9
   myUART6.write(x);                        // Send data over UART6
   //delay(2);                              // This is milliseconds, x1000 slower than microseconds
   delayMicroseconds(10000);                // start at 10000 and check the Serial monitor order.
                                            // uncheck autoscroll
                                            // mine messes up at 2 microseconds, fairly good
                                              
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
