/*
* Regular I2C both SDA and SCL pulled up to 3.3V with a low resistor say 10 ohms.
* Nano 33 BLE SDA = A4
* Nano 33 Ble SCL = A5
* GND = GND
*
*
*/




#include <Wire.h>

void setup()
{
  Wire.begin(1);                // join i2c bus with address #1
  Wire.onRequest(requestEvent); // register event
}

void loop()
{
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  Wire.write("I am one"); // respond with message of up to 9 bytes
                           // master set to receive up to 9 bytes 
}
