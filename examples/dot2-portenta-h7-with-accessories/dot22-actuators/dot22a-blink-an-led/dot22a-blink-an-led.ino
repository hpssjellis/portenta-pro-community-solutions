#include <Arduino.h> // Only needed by https://platformio.org/


void setup() {
   pinMode(D5, OUTPUT);  // just 5 if using the Seeeduino XIAO
   Serial.begin(115200);
}


void loop() {
   digitalWrite(D5, HIGH);      // sets LED on  
   Serial.println("Led connected to D5 ON");     
   delay(500);                  // waits 500ms (0.5 seconds)         
 
   digitalWrite(D5, LOW);       // sets LED off         
   Serial.println("D5 OFF");     
   delay(2000);                 // waits 2000ms (2 seconds)        
      
}


