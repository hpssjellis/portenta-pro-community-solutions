/* Portenta Serial UART

Lately this code seems to be broken. Note Really Sure why


// Need to connect breakout board UART1 RX to UART0 TX 


UART myUART0(PA_0,  PI_9,  NC, NC); //TX, RX, RTS, CTS  NOTE: NC means not connected
UART myUART1(PA_9,  PA_10, NC, NC);
UART myUART2(PG_14, PG_9,  NC, NC);
UART myUART3(PJ_8,  PJ_9,  NC, NC);

   // Serial1   is myUART1


*/
#include <Arduino.h>

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

#include <ArduinoJson.h>

//UART myUART0(PA_0,  PI_9,  NC, NC); //TX, RX, RTS, CTS  NOTE: NC means not connected
UART myUART1(PA_9,  PA_10, NC, NC);



void setup() { 
   bootM4();    
   Serial.begin(115200);
   myUART1.begin(115200); 
   delay(15000);
   Serial.print("Test Receive ArduoJSON receive UART RX TX between 2 Portenta M7 cores");
  
}

void loop() {
  // Check if the other Arduino is transmitting
  if (myUART1.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, myUART1);

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
  
      // Flush all bytes in the "link" serial port buffer
      // Is this needed ??
      while (myUART1.available() > 0)
        myUART1.read();
    }
  }
}







#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 
#include <ArduinoJson.h>
int  myCount=48;  //48 = ascii 0,    58 ascii 9

UART myUART0(PA_0,  PI_9,  NC, NC); //TX, RX, RTS, CTC  NOTE: NC means not connected
//UART myUART1(PA_9,  PA_10, NC, NC);


void setup() { 
    pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   // Serial.begin(115200);  // no serial monitor on M4 core without RPC
    myUART0.begin(115200);   

}

void loop() {
   // Values we want to transmit
  long timestamp = millis();
  int value = analogRead(A1);   // A0 is buggy

  // Print the values on the "debug" serial port
 // Serial.print("timestamp = ");
  //Serial.println(timestamp);
 // Serial.print("value = ");
 // Serial.println(value);
 // Serial.println("---");

  // Create the JSON document
  StaticJsonDocument<200> doc;
  doc["timestamp"] = timestamp;
  doc["value"] = value;

  // Send the JSON document over the "link" serial port
  serializeJson(doc, myUART0);

  // Wait
  delay(500);
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
