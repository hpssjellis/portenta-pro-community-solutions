/***************************************************
* DFRobot Gravity: Analog TDS Sensor / Meter For Arduino
* <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
*
* Created 2017-8-22
* By Jason <jason.ling@dfrobot.com@dfrobot.com>
*
* GNU Lesser General Public License.
* See <http://www.gnu.org/licenses/> for details.
* All above must be included in any redistribution
*
************Notice and Trouble shooting***************
* 1. This code is tested on Arduino Uno and Leonardo with Arduino IDE 1.0.5 r2 and 1.8.2.
* Also tested on the Arduino Portenta 
* 
* 2. More details, please click this link: <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
 *
 * Purchase here https://wiki.seeedstudio.com/Grove-OLED_Display_0.96inch/ about $20 USD
 *
 * Note u8g2 is an arduino search caplble library, easy to install
 *
 *  using 128 x 64 seeeduino OLED
 *  black GND 
 *  red 3v3      so do not need the protective resistors below
 *  white SDA    D11 // on portenta
 *  yellow SCL   D12 // on portenta
 *
 * another reference here 
 * https://github.com/olikraus/u8g2/wiki/u8g2reference#setfont
 *
 * For platformio you need the following at the bottom of the platformio.ini file
 *  lib_deps = olikraus/U8g2@^2.32.10
 *
****************************************************
*/
#include <Arduino.h>  // only needed for https://platformio.org/

#include <U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// works
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   //(rotation, clock, data [, reset])

// makes it 128 x 32 if you want that
//U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // SSD1306 and SSD1308Z are compatible
//
// Not yet tested, for Portenta Breakout Board
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, PH_7, PH_8,    /* reset=*/ U8X8_PIN_NONE);   // I2C0 default //(rotation, clock, data [, reset])
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, PB_6, PB_7,    /* reset=*/ U8X8_PIN_NONE);   // I2C1
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,  PH_11, PH_12, /* reset=*/ U8X8_PIN_NONE);   // I2C2
 






#define TdsSensorPin A1
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;

void setup(){
    Serial.begin(115200);
    u8g2.begin();
    pinMode(TdsSensorPin,INPUT);
}

void loop()
{
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");

      u8g2.clearBuffer();                   // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); 
      // FONTS to try    
      // u8g2_font_ncenB08_tr           // normal
      // u8g2_font_u8glib_4_tf          // small
      // u8g2_font_ncenB18_tr           // bigger
      // u8g2_font_pieceofcake_mel_tr   // weird
      // u8g2_font_ncenB08_tr           // skinny
      // u8g2_font_unifont_t_symbols    // stretched
      // FONT LIST at 
      //https://github.com/olikraus/u8g2/wiki/fntlistall
        
      // u8g2.setFontDirection(random(3)); // 0, normal, 1,2,3 rotate by 90 degrees
     // u8g2.drawPixel(random(128), random(64));
      
      u8g2.drawLine(0,0,  127, 0);        // horizontal top
      u8g2.drawLine(0,63, 127,63);        // horizontal bottom
      u8g2.drawLine(0,0,0,63);            // vertical left
      u8g2.drawLine(127,0,127,63);        // vertical right

     //u8g2.drawCircle(90,20, random(15)); // Circle center x,y,  random radius

      //char myOut[30] = "abcdefgh";
      char myOut[30];
      String s1 = "TDS: "+ String(tdsValue,0);
      s1.toCharArray(myOut,12);
   
      u8g2.drawStr(10,30,myOut);  // write something to the internal memory
      u8g2.sendBuffer();  
          
   }
}
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
