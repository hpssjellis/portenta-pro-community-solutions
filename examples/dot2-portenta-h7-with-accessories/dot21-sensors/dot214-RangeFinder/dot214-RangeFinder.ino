
 

// Note: VCC on RangeFinder must go to +5V on Portenta
//         GND on rangefinder goes to GND on Portenta 
    
#include <Arduino.h> // Only needed by https://platformio.org/


int myTriggerPin = D6;  // Trigger on RangeFinder
int myEchoPin = D7;     // Echo on Rangefinder  
unsigned long myDuration;

void setup() {
  Serial.begin(115200);
  pinMode(myTriggerPin, OUTPUT);
  pinMode(myEchoPin, INPUT_PULLDOWN);
}

void loop() {
  digitalWrite(myTriggerPin, LOW);
  delayMicroseconds(10); 
  digitalWrite(myTriggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(myTriggerPin, LOW);
  myDuration = pulseIn(myEchoPin, HIGH, 40000UL);
  Serial.println("Duration us: "+ String(myDuration));
  delay(200);
  if (myDuration > 2000    ){       // raw data from 200 to 16000                                          
                                        // where  2000 raw = ~35cm,  4000 raw = ~80cm                                    
       digitalWrite(LEDB, LOW);    // LEDB Blue LED on if far 
    } else { 
        digitalWrite(LEDB, HIGH);   // LEDB Blue LED off if close or nothing
    }  
}





// Note:  29 microseconds per centimeter.
// or     73.746 microseconds per inch

//reminder /2 as the signal goes out and back.
