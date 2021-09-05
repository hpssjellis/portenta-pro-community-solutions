#include <SPI.h>
#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>


#define ShowSerial Serial

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
                             //  both, both, analog, analog
//TouchScreen ts = TouchScreen(XP, YP, XM, YM); 
TouchScreen ts = TouchScreen(A3, A2, A1, A0);

void setup(void) {


    ShowSerial.begin(9600);
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library
    Tft.fillScreen(0, 240, 0, 320, BLUE);

    // no proof this text orientation works!
    TextOrientation orientation;
    orientation = PORTRAIT_VERTICAL; // LANDSCAPE; PORTRAIT_BACKWARDS, PORTRAIT_UPSIDE_DOWN_BACKWARDS,PORTRAIT_UPSIDE_DOWN, PORTRAIT_VERTICAL

}

void loop(void) {
    // a point object holds x y and z(pressure) coordinates
    Point p = ts.getPoint();

    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

    Tft.drawString("Cool" , 90, 140, 3, GREEN);

    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    
    if (p.z > __PRESSURE) {
   // if (p.z > 0) {
        ShowSerial.println("x: " + String(p.x) + ", y: " + String(p.y) + ", pressure: " + String(p.z));

        int myRadius = (500-p.z)/10;  // format pressure
        Tft.drawCircle(p.x, p.y, myRadius, random(0xFFFF));    //draw circle, center:(119, 160), color: random

    }

    delay(50);  // slow it down for no real reason
}
