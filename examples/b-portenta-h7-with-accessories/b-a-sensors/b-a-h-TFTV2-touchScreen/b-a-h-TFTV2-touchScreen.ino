//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
// needs this library  Adafruit_ILI9341
// needs this library install as zip file https://github.com/Lennart401/PortentaTouchScreen
//


#include <Adafruit_GFX.h>
//#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <PortentaTouchScreen.h>

//Touchscreen X+ X- Y+ Y- pins
// set For Portenta
#define XP D5   // can be a digital pin
#define YP A6  // must be an analog pin, use "An" notation!
#define XM A7  // must be an analog pin, use "An" notation!
#define YM D4   // can be a digital pin

//PortentaTouchScreen ts(XP, YP, XM, YM, 300);
//PortentaTouchScreen ts(A3, A2, A1, A0, 300);  // Yun
// must be:       digital, analog/digital, analog/digital, digital, resistor across x plate ~300 ohms
PortentaTouchScreen ts(D5, A6, A7, D4, 300);  // on Portenta

// Set for Portenta
#define TFT_CS D7
#define TFT_DC D6
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup(void)
{
  Serial.begin(9600);
  tft.begin();

  tft.fillScreen(ILI9341_BLUE);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1); 
  
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(100, 20);  //before print set x,y
  tft.println("Working!");

}

void loop(){
  
    // a point object holds x y and z(pressure) coordinates
    Point p = ts.getPoint();

    int myX      = map(p.x(), 600, 1800, 0, 240);   // map raw x to real x
    int myY      = map(p.y(), 600, 1400, 0, 320); // map raw y to real y
    int myRadius = map(p.z(), 100, 700, 0, 70); // map raw Pressure to real Radius
    
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    
     if (p.z() > 0) {  // minimum presure to register
      
        Serial.println(" raw x: " + String(p.x()) + ",\t  raw y: " + String(p.y()) + ",\t raw pressure: " + String(p.z()));
        Serial.println("real x: " + String(myX)   + ",\t real y: " + String(myY)   + ",\t real  radius: " + String(myRadius));
        Serial.println("");

        tft.drawCircle(myX, myY, myRadius, random(0xFFFF));    //draw circle, center:(119, 160), color: random
    }
    delay(100);  // slow it down to read serial monitor can speed it up when working
}


