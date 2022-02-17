#include <Arduino.h> // Only needed by https://platformio.org/


void setup() {
   pinMode(LED_BUILTIN, OUTPUT);  
}
void loop() {
    digitalWrite(LED_BUILTIN, LOW);    // sets LED on       
    delay(200);                        // waits 200ms (0.2 seconds)         
    digitalWrite(LED_BUILTIN, HIGH);   // sets LED off         
    delay(200);                        // waits 200ms (0.2 seconds)        
      
    // your code goes here!
   
    delay(5000);
}

    // make the LED flash an SOS then wait 5 seconds and repeat
    // Who can do it in as few lines as possible!!
    // each command on it's own line
