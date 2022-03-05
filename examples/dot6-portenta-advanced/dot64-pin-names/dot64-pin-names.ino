#include "pinDefinitions.h"


//String myPinNameFunction(int thePin){
//  return "P" + String(char('A' + (digitalPinToPinName(thePin) / 16))) +"_" + String(digitalPinToPinName(thePin) % 16);
//}

// thanks Arduino for the following
String myPinNameFunction(int thePin){
  int realPin = digitalPinToPinName(thePin) % ALT0;
  if (realPin == NC) {
    return "Not a pin";
  }
  String extra = "";
  switch (digitalPinToPinName(thePin) & DUAL_PAD) {
    case 0:
      break;
    case ALT0:
      extra = "ALT0";
      break;
    case ALT1:
      extra = "ALT1";
      break;
    case ALT2:
      extra = "ALT2";
      break;
    case DUAL_PAD:
      extra = "_C";
      break;
  }
  return "P" + String(char('A' + (realPin / 16))) +"_" + String(realPin % 16) + extra;
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
      Serial.println("Pin Index " + String(myLoop) + ": " + myName + ", Back to pin index: " + String(myIndex)+ "    " );
   }
   Serial.println("-----------------------------");
  
}

void loop() {
  
}
