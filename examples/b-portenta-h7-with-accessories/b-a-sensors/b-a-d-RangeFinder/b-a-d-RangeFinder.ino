
 

//T4A01-RangeFinder-Name.ino 

// EXAMPLE 
//using Ultrasonic Range Finder from Robotshop 

//http://www.robotshop.com/ca/en/hc-sr04-ultrasonic-range-finder.html 


unsigned long duration; 


void setup() { 
    Serial.begin(115200);
    pinMode(LEDB, OUTPUT); // LED on LEDB 
                           // ultrasonic range finder Robotshop RB-lte-54 
                           
                           // GND pin goes to ground 
    pinMode(D6, INPUT);    // echo 
    pinMode(D7, OUTPUT);   // Trig 
                           // VCC pin goes to VIN on the photon 
} 


void loop(){ 
        delay(10);                      // even cicuits need a break 
        digitalWrite(D7, HIGH);         // activate trigger 
        delayMicroseconds(10); 
        digitalWrite(D7, LOW);          // de-activate trigger 

        duration = pulseIn(D6, HIGH);   // how long until a reply? 
                                        // a blocking call so may wait a few seconds                    
        if (duration > 2000    ){       // raw data from 200 to 16000                                          
                                        // where  2000 raw = ~35cm,  4000 raw = ~80cm                                    
            digitalWrite(LEDB, LOW);    // LEDB Blue LED on if far 
        } else { 
            digitalWrite(LEDB, HIGH);   // LEDB Blue LED off if close or nothing
        }  
    Serial.println("Duration: " + String(duration)  );
}




// Note:  29 microseconds per centimeter.
// or     73.746 microseconds per inch
