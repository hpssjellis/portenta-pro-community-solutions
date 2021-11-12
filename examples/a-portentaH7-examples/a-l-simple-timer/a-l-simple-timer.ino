
int myDelay = 2000;   // non-block delay in milliseconds
unsigned long myStart; 
bool  mySwitch = false; 

void setup() {
   myStart = millis();   // set delay
   pinMode(LEDB, OUTPUT);
}

void loop() {
    if ( (millis() - myStart ) >= myDelay) {       
       myStart = millis();      //  reset the delay time
       mySwitch = !mySwitch;    // flip the switch ture <--> false
    }
    if (mySwitch){
       digitalWrite(LEDB, LOW); // turn the Portenta on-board LED on 
    } else {
       digitalWrite(LEDB, HIGH); // turn the Portenta on-board LED off
    }

}
