/* Portenta Serial UART
*
* ================================================================================
* For this program
* Simple one way com to m4 core
* 
* Connect myUART6 pin D5 TX to myUART8 pin D6 RX 
* 
* The LED's controlled by the M4 core should flash through colors quickly
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


UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX

int  myCount=48;  //48 = ascii 0,    58 ascii 9

void setup() { 
    bootM4();                // get the M4 core running   
    Serial.begin(115200);
                             // Note: Crash data (RED LED) also sent along this UART channel
    myUART6.begin(9600);     // same as myUART1  pin D14 TX, pin D13 RX  

}

void loop() {
   myCount++;
   if (myCount >= 58){myCount = 48;}        // 48 = ascii 0,    58 ascii 9
   char x = (char)myCount;                  // ascii code for numbers 0 to 9
   Serial.write(x);                         // serial monitor but proves nothing
   
   myUART6.write(x);                        // Send data over Serial1 to be received by myUART6
   //delay(100);                            // This is milliseconds, x1000 slower than microseconds
   delayMicroseconds(100000);                  // start at 100000 and check the Serial monitor order and board LED's
                                            // uncheck autoscroll
                                            // mine messes up at 2 microseconds, fairly good
                                              
}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

UART myUART8(NC,    PA_8, NC, NC);    // pin D6 RX only can receive

void setup() { 
   myUART8.begin(9600); 
   pinMode(LEDR, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red   
   pinMode(LEDG, OUTPUT);     
   pinMode(LEDB, OUTPUT);   

}

void loop() {
  if (myUART8.available()) {                  // If anything comes from Serial1 to myUART6 
    char x = (char)myUART8.read();            // Read it and set an LED;  48 = ascii 0,    58 ascii 9
    if (x == '0' ) {digitalWrite(LEDR, LOW) ; digitalWrite(LEDG, HIGH); digitalWrite(LEDB, HIGH);} // 0, Red
    if (x == '2' ) {digitalWrite(LEDR, HIGH); digitalWrite(LEDG, LOW) ; digitalWrite(LEDB, HIGH);} // 2, Green
    if (x == '6' ) {digitalWrite(LEDR, HIGH); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, LOW) ;} // 6, Blue
    if (x == '8' ) {digitalWrite(LEDR, LOW) ; digitalWrite(LEDG, LOW) ; digitalWrite(LEDB, LOW) ;} // 8, White
    // Reminder for modern onboard LED's  LOW is on 

  }

}
#endif

//////////////////// End All Core M4 Programing /////////////////////
