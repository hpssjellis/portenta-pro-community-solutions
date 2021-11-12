
int myDelay= 2000;   // non-block delay in milliseconds
unsigned long delayStart = 0;  

void setup() {
   delayStart = millis();   // set delay
   pinMode(LEDB, OUTPUT);
}

void loop() {
    if ( (millis() - delayStart ) >= myDelay) {       
       delayStart = millis(); //  reset the delay time
       digitalWrite(LEDB, LOW); // turn the Portenta on-board LED on 
    } else {
       digitalWrite(LEDB, HIGH); // turn the Portenta on-board LED off
    }

}
