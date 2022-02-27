/* Portenta Serial UART
*
* ================================================================================
* For this program
* 
* Connect Serial1 pin D14 TX to myUART6 pin D4 RX 
* Connect Serial1 pin D13 RX to myUART6 pin D5 TX 
* 
* The LED's controlled by the M4 core should flash through colors quickly
* This time 2 way communication!
* 
* Note: For 2 way communication we need non-blocking timers 
* If not, delayMicroseconds() would have messed things up
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

int myCountM7=48;  //48 = ascii 0,    58 ascii 9
unsigned long myDelayM7 = 100000;   // non-block delay in microseconds. Try 100 works fine
unsigned long myStartM7; 

void setup() { 
    bootM4();                // get the M4 core running   
    Serial.begin(115200);
                             // Note: Crash data (RED LED) also sent along this UART channel
    Serial1.begin(9600);     // same as myUART1  pin D14 TX, pin D13 RX  
    myStartM7 = micros();    // set start
}


void loop() {
    unsigned long myNowM7 = micros();
   if ( (myNowM7 - myStartM7 ) >= myDelayM7) {  
     myStartM7 = myNowM7;    // reset start
     myCountM7++;
     if (myCountM7 > 58){myCountM7 = 48;}        // 48 = ascii 0,    58 ascii 9
       char xM7 = (char)myCountM7;      
       Serial1.write(xM7);                        // Send data over Serial1 to be received by myUART6
   }
                                          
   if (Serial1.available()) {               // if anything sent from myUART6 to Serial 1
     Serial.write(Serial1.read());          // Show it on the serial monitor
   }                                         
}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX
int myCountM4=65;                     // 65 = ascii A,    90 ascii Z
unsigned long myDelayM4 = 100000;     // non-block delay in microsseconds, try 100 works fine
unsigned long myStartM4; 

void setup() { 
   myUART6.begin(9600); 
   pinMode(LEDR, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red   
   pinMode(LEDG, OUTPUT);     
   pinMode(LEDB, OUTPUT);   
   myStartM4 = micros();   // set start
}

void loop() {
  if (myUART6.available()) {                     // If anything comes from Serial1 to myUART6 
    char myIn = (char)myUART6.read();            // Read it and set an LED;  48 = ascii 0,    58 ascii 9
    if (myIn == '0' ) {digitalWrite(LEDR, LOW) ; digitalWrite(LEDG, HIGH); digitalWrite(LEDB, HIGH); } // 0, Red
    if (myIn == '2' ) {digitalWrite(LEDR, HIGH); digitalWrite(LEDG, LOW) ; digitalWrite(LEDB, HIGH); } // 2, Green
    if (myIn == '6' ) {digitalWrite(LEDR, HIGH); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, LOW) ; } // 6, Blue
    if (myIn == '8' ) {digitalWrite(LEDR, LOW) ; digitalWrite(LEDG, LOW) ; digitalWrite(LEDB, LOW) ; } // 8, White
    // Reminder for modern onboard LED's  LOW is on 
  }

   unsigned long myNowM4 = micros();
   if ( (myNowM4 - myStartM4 ) >= myDelayM4) {    // now sending from myUART6 to Serial1
     myStartM4 = myNowM4;   // reset start
     myCountM4++;
     if (myCountM4 > 90){myCountM4 = 65;}          // 65 = ascii A,    90 ascii Z
       char myOut = (char)myCountM4;      
       myUART6.write(myOut);                       // Send data over Serial1 to be received by myUART6
   }

  

}
#endif

//////////////////// End All Core M4 Programing /////////////////////
