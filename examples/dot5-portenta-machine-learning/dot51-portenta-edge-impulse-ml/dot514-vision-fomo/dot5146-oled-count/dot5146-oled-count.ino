/* Edge Impulse Arduino examples
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * 
 *  FOR the GRAYSCALE Waveshare OLED 128 x 128 using library Adafruit_SSD1327.h
 *   blue  DIN (mosi) D8
 *   yellow (sck) D9                                                                                                                                                             
 *   orange (cs) D7
 *   green (dc)  D6
 *   white (reset) not needed but D14 if you did
 *
 * another reference here 
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 * Note1: Should work with any Edge Impulse model just change the below include to your model name
 */


 
/* Includes ---------------------------------------------------------------- */


#include <ei-v1-1-minst-96x96_inferencing.h>

//#include <ei-v6-0-1-fomo-2.8.0mbed-96x96-vision-1pop_inferencing.h>
#include <map>


#include "edge-impulse-advanced-v2.h"
#include <Adafruit_SSD1327.h>

// Used for software SPI
#define OLED_CLK D9
#define OLED_MOSI D8

// Used for software or hardware SPI
#define OLED_CS D7
#define OLED_DC D6

// Used for I2C or SPI
#define OLED_RESET -1

// hardware SPI
Adafruit_SSD1327 display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Global Variables
int myDelay = 0;  // delay between readings, can be zero, default 2000 = 2 seconds
int x1Map, x2Map, y1Map, y2Map;
//int myClassCount[EI_CLASSIFIER_LABEL_COUNT]; // not yet used

// the OLED uses these
#define CUTOUT_COLS                 EI_CLASSIFIER_INPUT_WIDTH
#define CUTOUT_ROWS                 EI_CLASSIFIER_INPUT_HEIGHT
const int cutout_row_start = (EI_CAMERA_RAW_FRAME_BUFFER_ROWS - CUTOUT_ROWS) / 2;
const int cutout_col_start = (EI_CAMERA_RAW_FRAME_BUFFER_COLS - CUTOUT_COLS) / 2;


int myTotalObjects = 0;
std::map<std::string, int> cnt;




/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
  //  Serial.println("Edge Impulse Inferencing Demo");

    pinMode(LEDR, OUTPUT); 
    pinMode(LEDG, OUTPUT);   // this is LED_BUILTIN
    pinMode(LEDB, OUTPUT); 
    
#ifdef EI_CAMERA_FRAME_BUFFER_SDRAM
    // initialise the SDRAM
    SDRAM.begin(SDRAM_START_ADDRESS);
#endif

    if (ei_camera_init()) {
       // Serial.println("Failed to initialize Camera!");
    }
    else {
      //  Serial.println("Camera initialized");
    }

    for (size_t ix = 0; ix < ei_dsp_blocks_size; ix++) {
        ei_model_dsp_t block = ei_dsp_blocks[ix];
        if (block.extract_fn == &extract_image_features) {
            ei_dsp_config_image_t config = *((ei_dsp_config_image_t*)block.config);
            int16_t channel_count = strcmp(config.channels, "Grayscale") == 0 ? 1 : 3;
            if (channel_count == 3) {
                //Serial.println("WARN: You've deployed a color model, but the Arduino Portenta H7 only has a monochrome image sensor. Set your DSP block to 'Grayscale' for best performance.");
                break; // only print this once
            }
        }
    }
    
    // Following for the Grayscale OLED

   if ( ! display.begin(0x3D) ) {   // start Grayscale OLED
    // Serial.println("Unable to initialize OLED");
     while (1) yield();
  }    
    display.setTextSize(1);
    display.setTextColor(SSD1327_WHITE);

    display.setRotation(0);
    display.setCursor(0,0);

    //   map cutout of the 320 x 320   // 240 model to OLED 128 x 64 screen
    x1Map = map((int)cutout_col_start, 0, 320, 0, 127);  
    x2Map = map((int)CUTOUT_COLS, 0, 320, 0, 127);
    y1Map = map((int)cutout_row_start, 0, 320, 0, 127);
    y2Map = map((int)CUTOUT_ROWS, 0, 320, 0, 127);

    
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop(){
  
    // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
    if (ei_sleep(myDelay) != EI_IMPULSE_OK) {
        return;
    }
    
    //Serial.println("Starting inferencing in "+String(myDelay)+" microseconds...");

    // Put the image on the OLED
    display.clearDisplay();                 // clear the internal memory for OLED
    for (int x=0; x < EI_CAMERA_RAW_FRAME_BUFFER_COLS; x++){     // EI_CAMERA_RAW_FRAME_BUFFER_COLS = 320
      for (int y=0; y < EI_CAMERA_RAW_FRAME_BUFFER_ROWS; y++){       //EI_CAMERA_RAW_FRAME_BUFFER_ROWS = 320   //240
      
        uint8_t myGRAY = ei_camera_frame_buffer[(y * (int)EI_CAMERA_RAW_FRAME_BUFFER_COLS) + x];  

          int myGrayMap = map(myGRAY, 0, 255, 0, 15);  
          int xMap = map(x, 0, 320, 0, 127);
          int yMap = map(y, 0, 320, 0, 127);
          display.drawPixel(xMap, yMap, myGrayMap );   // grayscale 0-255, 128x128  //128 x 64
      } 
    }

    display.drawRect(2, 2,   126, 126, SSD1327_WHITE );  // rectangle around outside of OLED

    display.setCursor(20,5);
    display.println("Rocksetta: ");


    
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_cutout_get_data;

    if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
      //  Serial.println("Failed to capture image\r\n");
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
      //  Serial.println("ERR: Failed to run classifier, error number: " + String(err));
        return;
    }

  

    bool bb_found = result.bounding_boxes[0].value > 0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++) {
        auto bb = result.bounding_boxes[ix];
        if (bb.value == 0) {
            continue;
        }

        std::string label(bb.label);
        auto cnt_entry = cnt.find(label);
        if (cnt_entry == cnt.end()) {
            cnt[label] = 1;
        }
        else {
            cnt_entry->second++;
        }


       // ei_printf("    %s (", bb.label);
       // ei_printf_float(bb.value);
      //  ei_printf(") [ x: %u, y: %u, width: %u, height: %u ]\n", bb.x, bb.y, bb.width, bb.height);
        display.setCursor(bb.x, bb.y-5);
        display.println(String(bb.label).substring(0, 4) );  // only print the start of the label

        int xMap = map(bb.x, 0,96, 0,127);
        int yMap = map(bb.y, 0,96, 0,127);
        int widthMap = map(bb.width, 0,96, 0,127);
        int heightMap = map(bb.height, 0,96, 0,127);
        
        display.drawRect(xMap, yMap, widthMap, heightMap, SSD1327_WHITE ); 
        myTotalObjects += EI_CLASSIFIER_OBJECT_DETECTION_COUNT;



    }

    if (!bb_found) {
       // ei_printf("    No objects found\n");
    }

    // print the counts per label
    int myLoop = 2;
    for (auto it : cnt) {
       // printf("%s: %d\n", it.first.c_str(), it.second);
       //display.setCursor(80,5); // near the top
       //display.println(String(myTotalObjects));
        myLoop++;
        display.setCursor(5,(myLoop*10)); 
        display.println(it.first.c_str());  // label    
        display.setCursor(15,(myLoop*10)); 
        display.println(String(it.second)); // count for that label     
    }

    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
   // Serial.println("    anomaly score: " + String(result.anomaly, 5));
#endif
   
    digitalWrite(LEDB, HIGH);   //on board LED's are turned off by HIGH    
    digitalWrite(LEDG, HIGH);   
    digitalWrite(LEDR, HIGH); 

   if (bb_found) {    // if objects are found lets load some lights
     if (EI_CLASSIFIER_OBJECT_DETECTION_COUNT % 3 == 0) { digitalWrite(LEDR, LOW); } // red on
     if (EI_CLASSIFIER_OBJECT_DETECTION_COUNT % 3 == 1) { digitalWrite(LEDG, LOW); } // green on
     if (EI_CLASSIFIER_OBJECT_DETECTION_COUNT % 3 == 2) { digitalWrite(LEDB, LOW); } // blue on
   }








        display.setCursor(80,5); // near the top
        display.println(String(myTotalObjects));
        
     // Last thing is to show the 128x128 GRAYSCALE OLED
     display.display();  // OLED 4 bit 16 color GRAYSCALE update
}
