/*
*
* Master for this I2C will be the Portenta
* Portenta SDA on D11
Portneta SCL on D12
*
*
*
*/

#include <Wire.h>

bool myIncoming = false;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop()
{
  Wire.requestFrom(1, 9);    // request 9 bytes from slave device #1
  
  while(Wire.available()){    // slave may send less than requested
    myIncoming = true;
    char myChar = Wire.read(); // receive a byte as character
    Serial.print(myChar);      // print the character
  }
  if (myIncoming) {
      Serial.println();        // just nicely finishes the line.
      myIncoming = false;
    }
    Serial.println("Waiting...");
  delay(1000);
}
