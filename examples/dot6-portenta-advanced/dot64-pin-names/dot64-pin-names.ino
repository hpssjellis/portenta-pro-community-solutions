#include "pinDefinitions.h"


String myPinNameFunction(int thePin){
  return "P" + String(char('A' + (digitalPinToPinName(thePin) / 16))) +"_" + String(digitalPinToPinName(thePin) % 16);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}  // wait for Serial

   // This tests each pinName converts to pin index
   // Serial.println("Pin PH_15: " + String(PinNameToIndex(PH_15)) );

   // This shows all index's as pin names
   for (int myLoop=0; myLoop<=195; myLoop++){
      String myName = myPinNameFunction(myLoop);
      PinName myIn  = digitalPinToPinName(myLoop);
      int myIndex   = PinNameToIndex(myIn);
      Serial.println("Pin Index " + String(myLoop) + ": " + myName + ", Back to pin index: " + String(myIndex) );
   }
   Serial.println("-----------------------------");
  
}

void loop() {
  
}
