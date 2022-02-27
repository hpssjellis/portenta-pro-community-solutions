/* Portenta Serial UART
*
* ================================================================================
* For this program
* 
* 1 way
* Connect (sender) myUART6 pin D5 TX to (receiver) myUART8 pin D6 RX 
* 
* 
* If you wanted to do 2 way
* Connect Serial1 pin D14 TX to myUART6 pin D4 RX 
* Connect Serial1 pin D13 RX to myUART6 pin D5 TX 
* 
* 
* Then load serial monitor, change delay to speed up
* 
* ================================================================================
* 
* * UART Information for the PortentaH7 
* 
* UART myUART0(PA_0,  PI_9,  NC, NC);  // TX, RX, RTS, CTC  NOTE: NC means not connected
* UART myUART1(PA_9,  PA_10, NC, NC);   // pin D14 TX, pin D13 RX same as pre-defined Serial1
* UART myUART2(PG_14, PG_9,  NC, NC);
* UART myUART3(PJ_8,  PJ_9,  NC, NC);
* UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX
* UART myUART8(NC,    PA_8, NC, NC);    // pin D6 RX only can receive
* 
*/


#ifdef CORE_CM7  
     
#include "SerialTransfer.h"

UART myUART8(NC, PA_8, NC, NC);    // pin D6 RX only can receive

SerialTransfer myTransfer;

struct STRUCT {
  char  z;
  float y;
} testStruct;

char arr[6];


void setup()
{
  bootM4();
  Serial.begin(115200);
  myUART8.begin(9600);  

  myTransfer.begin(myUART8);
}


void loop()
{
  if(myTransfer.available())
  {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    recSize = myTransfer.rxObj(testStruct, recSize);
    Serial.print(testStruct.z);
    Serial.print(testStruct.y);
    Serial.print(" | ");

    recSize = myTransfer.rxObj(arr, recSize);
    Serial.println(arr);
  }
}

    
#endif



#ifdef CORE_CM4  

#include "SerialTransfer.h"

UART myUART6(PC_6,  PC_7, NC, NC);    // pin D5 TX, pin D4 RX

SerialTransfer myTransfer;

struct STRUCT {
  char z;
  float y;
} testStruct;

char arr[] = "hello";


void setup()
{
 // Serial.begin(115200);
  myUART6.begin(9600);
  myTransfer.begin(myUART6);

  testStruct.z = '$';

}


void loop()
{
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  
  testStruct.y = random(1,900)/10.0;
  uint16_t sendSize = 0;

  ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(testStruct, sendSize);

  ///////////////////////////////////////// Stuff buffer with array
  sendSize = myTransfer.txObj(arr, sendSize);

  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);
  delayMicroseconds(5000);
}
     
#endif
