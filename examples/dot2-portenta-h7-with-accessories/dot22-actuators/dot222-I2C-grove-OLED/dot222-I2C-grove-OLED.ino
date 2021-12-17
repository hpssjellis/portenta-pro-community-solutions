/*  
 *  b-grove-OLED.ino
 *
 *
 * GNU GENERAL PUBLIC LICENSE
 * Use at your own risk.
 ************************************************ Important stuff if needed ****************************************
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
 
 ********************************************************** end ****************************************************
 *
 * 
 *  By Jeremy Ellis twitter @rocksetta
 *  Webpage http://rocksetta.com
 *  Arduino High School Robotics Course at
 *  https://github.com/hpssjellis/arduino-high-school-robotics-course
 * 
 *  Update March 28, 2021
 *
 * MbedI2C  myI2C0(PH_8,  PH_7);  //MbedI2C(int sda, int scl);
 * MbedI2C  myI2C1(PB_7,  PB_6); 
 * MbedI2C  myI2C2(PH_12, PH_11); 
 * 
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
 
void setup(void) {
  u8g2.begin();

}
 
void loop(void) {
  
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
  u8g2.drawPixel(random(128), random(64));
  
  u8g2.drawLine(0,0,  127, 0);        // horizontal top
  u8g2.drawLine(0,63, 127,63);        // horizontal bottom
  u8g2.drawLine(127,0,127,63);        // vertical right

  u8g2.drawCircle(90,20, random(15)); // Circle center x,y,  random radius
  u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
  u8g2.sendBuffer();                  // transfer internal memory to the display
  delay(1000);  
}







/*
* Clickable links for helpful information
* By @rocksetta
* March, 2021
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
*
*
*
*  Artduino Pro Links:
*
*  https://store.arduino.cc/usa/portenta-h7
*  https://forum.arduino.cc/index.php?board=148.0
*  https://www.arduino.cc/pro/tutorials/portenta-h7
*
*  Rocksetta links:
* 
*  https://twitter.com/rocksetta
*  https://github.com/hpssjellis/portenta-pro-community-solutions
*  https://github.com/hpssjellis/my-examples-for-the-arduino-portentaH7
*  https://github.com/hpssjellis/arduino-high-school-robotics-course
*  https://www.youtube.com/playlist?list=PL57Dnr1H_egtm0pi-okmG0iE_X5dROaLw
*
*
*
*/
