// a10-transitor
// By Jeremy Ellis
// MIT License


// Transistors can protect the Portenta from hgiher voltages and currents 
// while still allowing the Portenta to control the Higher Power circuits.

// In a PNP Transistor the large Voltage and Current circuit connects it's Ground to the to Bottom Emitter (P)
// In a PNP transistor the Portenta Ground (GND) goes to the BASE (N)
// In a PNP transistor  both circuits connect their Positives (Pins on the Portenta) to the Top Collector (P)

// Trick here, the middle letter is always what the Portenta connects to. 
// PNP transistor the Portenta connects ground which is negative to the (N) 
// NPN transistor the Portenta connects pins which are positive to the (P) 
// You can test the Portneta pin by just using a 3.3 V battery (2 x 1.5 V)

// In a NPN Transistor the large Voltage connects to the top (N)
// In a NPN transistor the Portenta Pins go to the (p)
// In a NPN transistor both circuits connect their Negatives to the bottom (N) 



#include <Arduino.h> // Only needed by https://platformio.org/

void setup() {
   Serial.begin(115200);
   pinMode(D5, OUTPUT);  //No PWM-2 labelled pin use D5 pin on Portenta
   // Do not use Analog pins A2 etc as they are for reading analog not sending analog (PWM)

   pinMode(LED_BUILTIN, OUTPUT);
   while(!Serial){;}  // Blocking, only use if needed
   delay(9000);
   Serial.println("Serial Monitor Connected");
}

void loop() {
    analogWrite(D5,100);
    digitalWrite(LED_BUILTIN, LOW);  // internal LED's on Portenta set to LOW turns them on!
    Serial.println("Slow");
    delay(2000);
    
    analogWrite(D5,0);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Stopped");
    delay(1000);
        
    analogWrite(D5,200);  // PWM Max  = 256
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Fast");
    delay(2000);
    
    analogWrite(D5,0);
    digitalWrite(LED_BUILTIN, HIGH); 
    Serial.println("Stop and wait");
    delay(5000);

}