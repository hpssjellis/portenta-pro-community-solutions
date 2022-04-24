/*
 * 
 * Must use portenta with Vision shield camera and Waveshare Grayscale 128 x 128 OLED
 * Should be implemented with MBED version greater than 2.8.0
 *
 * Purchase here https://www.waveshare.com/1.5inch-OLED-Module.htm about $29 USD
 *
 *
 *  FOR the GRAYSCALE Waveshare OLED
 *   black GND 
 *   red 3v3     
 *   blue  DIN (mosi) D8
 *   yellow (sck) D9 
 *   orange (cs) D7
 *   green (dc)  D6
 *   white (reset) not needed but D14 if you did
 *
 * another reference here 
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * Use at your own risk!
 * By @rocksetta
 * MIT license
 *
 *
 */
 
 
#include <Arduino.h>  // only needed for https://platformio.org/

#include <Adafruit_SSD1327.h>  // for OLED

#include "camera.h"
#include "himax.h"

// Used for software SPI
#define OLED_CLK D9  //yellow wire
#define OLED_MOSI D8 // blue wire 

// Used for software or hardware SPI
#define OLED_CS D7  // orange wire
#define OLED_DC D6   // green wire 

// Used for I2C or SPI
#define OLED_RESET -1

// hardware SPI
Adafruit_SSD1327 display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);


HM01B0 himax;
Camera cam(himax);

/*
Other buffer instantiation options:
  FrameBuffer fb(0x30000000);
  FrameBuffer fb(320,320,2);
*/

FrameBuffer fb;

static uint8_t frame_buffer[320*320] __attribute__((aligned(32)));

void setup() {
  Serial.begin(115200);  

  // Init the cam 
  //cam.begin(CAMERA_R320x320, CAMERA_GRAYSCALE, 30);
  cam.begin(CAMERA_R320x320, CAMERA_GRAYSCALE, 60);  // 60 FPS ?

  if ( ! display.begin(0x3D) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }    
    display.setTextSize(1);
    display.setTextColor(SSD1327_WHITE);

    display.setRotation(0);
    display.setCursor(0,0);

    // set the frame_buffer to the static uint8_t
    fb.setBuffer(frame_buffer); 
}

void loop() {
  display.clearDisplay();                 // clear the internal memory
    
  if (cam.grabFrame(fb, 3000) == 0) {
     // Serial.write(fb.getBuffer(), cam.frameSize());


    for (int x=0; x < 320; x++){     // FRAME_BUFFER_COLS = 320
       for (int y=0; y < 320; y++){       //FRAME_BUFFER_ROWS = 320
          uint8_t myGRAY = frame_buffer[(y * 320) + x];  

          int myGrayMap = map(myGRAY, 0, 255, 0, 15);  
          int xMap = map(x, 0, 320, 0, 127);
          int yMap = map(y, 0, 320, 0, 127);
          display.drawPixel(xMap, yMap, myGrayMap );   // grayscale 0-255, 128x128  //128 x 64
      } 
    }     
  }

  display.drawRect(0,0,128,128, SSD1327_WHITE ); // border around the screen
  display.setCursor(3,3);
  display.println("Rocksetta");
    
  display.display();
       
}
