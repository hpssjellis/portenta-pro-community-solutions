// dual motor driver
// By Jeremy Ellis
// MIT license


// info here until improved https://github.com/hpssjellis/particle.io-photon-high-school-robotics/tree/master/a11-dc-motor-drivers



// You are suppossed to get it working using the web-app
// Draw your circuit diagram first
// This program will just tell you if  the connections are working
// See https://www.pololu.com/product/2135 for assistance

void setup() {
    pinMode(D5, OUTPUT);   // PWM 0 to 256
    pinMode(D4, OUTPUT);   // digital 0 to 1
    pinMode(LEDB, OUTPUT);

}

void loop() {
    
    digitalWrite(LEDB, 0);    // LEDB on internal LED pulled LOW to go on
    digitalWrite(D4, 1);    // set one direction
    analogWrite(D5, 50);   // go medium
    delay(3000);
    
    analogWrite(D5, 0);     // stop
    delay(3000); 
   
    digitalWrite(D4, 0);    // set the other direction
    analogWrite(D5, 50);   // go medium   
    delay(1000);    
    
    analogWrite(D5, 255);   // go full speed
    delay(1000);
    
    analogWrite(D5, 0);     // stop
    
    digitalWrite(LEDB, 1);    // LEDB off
    delay(9000);            // wait 9 seconds
        
}
