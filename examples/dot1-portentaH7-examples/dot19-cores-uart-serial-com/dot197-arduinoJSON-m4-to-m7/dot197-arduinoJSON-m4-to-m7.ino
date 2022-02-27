/* Portenta Serial UART
*
* ================================================================================
* For this program
* 
* 1 way
* Connect (sender) myUART6 pin D5 TX to (receiver) myUART8 pin D6 RX 
* 
* 
* If you wanted to do 2 way
* Connect Serial1 pin D14 TX to myUART6 pin D4 RX 
* Connect Serial1 pin D13 RX to myUART6 pin D5 TX 
* 
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


#include <Arduino.h>

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

#include <ArduinoJson.h>

UART myUART8(NC,    PA_8, NC, NC);    // pin D6 RX only can receive

void setup() { 
   bootM4();    
   Serial.begin(115200);
   myUART8.begin(115200); 
   delay(15000);
   Serial.print("Test Receive ArduoJSON receive UART RX TX between 2 Portenta M7 cores");
  
}

void loop() {
  // Check if the other Arduino is transmitting
  if (myUART8.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, myUART8);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("timestamp = ");
      Serial.println(doc["timestamp"].as<long>());
      Serial.print("value = ");
      Serial.println(doc["value"].as<int>());
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "linked" serial port buffer
      // Is this needed ??
      while (myUART8.available() > 0)
        myUART8.read();
    }
  }
}



#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 
#include <ArduinoJson.h>

UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX

int  myCount=48;  //48 = ascii 0,    58 ascii 9

void setup() { 
    pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   // Serial.begin(115200);  // no serial monitor on M4 core without RPC
    myUART6.begin(115200);   

}

void loop() {
   digitalWrite(LEDB, !digitalRead(LEDB));  // switch Blue LED on / off
   
   // Values we want to transmit
  long timestamp = millis();
  int value = analogRead(A1);   // A0 is buggy

  // Create the JSON document
  StaticJsonDocument<200> doc;
  doc["timestamp"] = timestamp;
  doc["value"] = value;

  // Send the JSON document over the "linked" serial port
  serializeJson(doc, myUART6);

  // Wait
  //delay(100);
  delayMicroseconds(100000);      // set this smaller to see how fast it can go
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
