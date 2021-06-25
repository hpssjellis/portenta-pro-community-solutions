// SOS Programming your Photon
// By Jeremy Ellis


void setup() {
   pinMode(D7, OUTPUT);
  
}

void loop() {

    digitalWrite(D7, 1);          // sets LED on       
    delay(200);                   // waits 200ms (0.2 seconds)         
    digitalWrite(D7,0);           // sets LED off         
    delay(200);                   // waits 200ms (0.2 seconds)        
    
    
    // your programming goes here
    // make D7 flash an SOS then wait 5 seconds and repeat

    delay(5000);
}
