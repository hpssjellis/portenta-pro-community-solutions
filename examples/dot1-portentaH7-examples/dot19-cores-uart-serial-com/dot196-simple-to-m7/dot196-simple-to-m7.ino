/* Portenta Serial UART
*
* ================================================================================
* For this program
* Simple from m4 to m6
* 
* Connect myUART6 pin D5 TX to myUART8 pin D6 RX 
* 
* Then load serial monitor, change delay to speed up
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


UART myUART8(NC, PA_8, NC, NC);    // pin D6 RX only can receive

void setup() { 
   bootM4(); // get the M4 core running   
   Serial.begin(115200); // regular SerialUSB for printing to monitor
   myUART8.begin(9600);  // same as myUART1  pin D14 TX, pin D13 RX
}

void loop() {
  if (myUART8.available()) {          // If anything comes in Serial1 
     Serial.write(myUART8.read());    // Read it and send it out over SerialUSB to the monitor
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
   //delay(10);                             // This is milliseconds, x1000 slower than microseconds
   delayMicroseconds(100000);                // start at 10000 and check the Serial monitor order.
                                            // uncheck autoscroll
                                            // mine messes up at 2 microseconds, fairly good
                                              
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
