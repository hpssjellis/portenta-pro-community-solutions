//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
// needs this library  Adafruit_ILI9341
// needs this library install as zip file https://github.com/Lennart401/PortentaTouchScreen
//
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
//#include <TouchScreen.h>
#include <PortentaTouchScreen.h>

//Touchscreen X+ X- Y+ Y- pins
// set For Portenta
#define XP D5   // can be a digital pin
#define YP A6  // must be an analog pin, use "An" notation!
#define XM A7  // must be an analog pin, use "An" notation!
#define YM D4   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
PortentaTouchScreen ts(XP, YP, XM, YM);

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
  tft.setCursor(100, 20);  //x,y
  tft.println("Cool");

 // tft.drawCircle(100, 100, 40, random(0xFFFF));    //draw circle, center:(119, 160), color: random

}

void loop(){
  
    // a point object holds x y and z(pressure) coordinates
    Point p = ts.getPoint();

    int myX = map(p.x(), TS_MINX, TS_MAXX, 0, 240);
    int myY = map(p.y(), TS_MINY, TS_MAXY, 0, 320);
    


    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    
     if (p.z() > 0) {
        Serial.println("x: " + String(myX) + ", y: " + String(myY) + ", pressure: " + String(p.z()));

        //int myRadius = (500-p.z())/10;  // format pressure
        int myRadius = p.z();  // format pressure
        tft.drawCircle(myX, myY, 30, random(0xFFFF));    //draw circle, center:(119, 160), color: random
      // tft.drawCircle(10, 10, 10, random(0xFFFF)); 
    }
    delay(50);  // slow it down for no real reason
}


