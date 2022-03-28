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
 */

/* Includes ---------------------------------------------------------------- */

#include <ei-v6-0-1-fomo-2.8.0mbed-96x96-vision-1pop_inferencing.h>

//#include <ei-v20unknown-1popGoRight-2waterGoLeft-3fast-v2-0-0_inferencing.h>
#include "edge-impulse-advanced.h"

// Global Variables
int myDelay = 400;  // delay between readings, can be zero, default 2000 = 2 seconds



/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(LEDR,OUTPUT);
    pinMode(LEDG,OUTPUT);
    pinMode(LEDB,OUTPUT);
    Serial.println(F("Edge Impulse Inferencing Demo"));

#ifdef EI_CAMERA_FRAME_BUFFER_SDRAM
    // initialise the SDRAM
    SDRAM.begin(SDRAM_START_ADDRESS);
#endif

    if (ei_camera_init()) {
        Serial.println(F("Failed to initialize Camera!"));
    }
    else {
        Serial.println(F("Camera initialized"));
    }

    for (size_t ix = 0; ix < ei_dsp_blocks_size; ix++) {
        ei_model_dsp_t block = ei_dsp_blocks[ix];
        if (block.extract_fn == &extract_image_features) {
            ei_dsp_config_image_t config = *((ei_dsp_config_image_t*)block.config);
            int16_t channel_count = strcmp(config.channels, "Grayscale") == 0 ? 1 : 3;
            if (channel_count == 3) {
                Serial.println(F("WARN: You've deployed a color model, but the Arduino Portenta H7 only has a monochrome image sensor. Set your DSP block to 'Grayscale' for best performance."));
                break; // only print this once
            }
        }
    }
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop()
{
    //Serial.println("Starting inferencing in "+String(myDelay)+" microseconds...");

    // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
    if (ei_sleep(myDelay) != EI_IMPULSE_OK) {
        return;
    }

    Serial.println(F("Taking photo..."));

    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_cutout_get_data;

    if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
        Serial.println(F("Failed to capture image"));
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        Serial.print(F("ERR: Failed to run classifier, error number: "));
        Serial.println(err);
        return;
    }


    int myBestClassificationNumber = -1;  
    float myBestClassificationValue = 0.25;   // lowest best allowable value

    // find the largest value looping through all of them
    for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++) {
        if (result.bounding_boxes[ix].value > myBestClassificationValue ){
           myBestClassificationNumber = ix;                      // find the biggest array value
           myBestClassificationValue = result.bounding_boxes[ix].value;  
        }
    }

    
    // print the predictions
    Serial.println(F("Predictions "));
    // For complex prints best to run Edge Impulse ei_printf
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");

    // new code for FOMO bounding box edge impulse models
    // if no value in the first box then nothing in any of the boxes
    bool bb_found = result.bounding_boxes[0].value > 0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++) {
        auto bb = result.bounding_boxes[ix];
        if (bb.value == 0) {
            continue;
        }
        if (ix == myBestClassificationNumber){
            ei_printf("TOP VALUE: ");      
         }   
               
         ei_printf("    %s (", bb.label);
         ei_printf_float(bb.value);
         ei_printf(") [ x: %u, y: %u, width: %u, height: %u ]\n", bb.x, bb.y, bb.width, bb.height);
 
    }

    if (!bb_found) {
        ei_printf("    No objects found\n");
    }



    

   // how the above is done for classifying exported edge impulse models 
   // for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    //    ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
   // }


    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    Serial.print("    anomaly score: ");
    Serial.println(result.anomaly);
#endif


    digitalWrite(LEDB, HIGH);   //on board LED's are turned off by HIGH    
    digitalWrite(LEDG, HIGH);   
    digitalWrite(LEDR, HIGH); 

    // I find it less confusing if the 0unknown does nothing
    if (myBestClassificationNumber == 0){    // 0 unknown do nothing
        digitalWrite(LEDB, LOW);    
        digitalWrite(LEDG, LOW);   
      //  ei_printf("0: Unknown Stop: %.5f\n", myBestClassificationValue);
    }

    if (myBestClassificationNumber == 1){   // 1pop: Go Right
      digitalWrite(LEDB, LOW);              // Blue LED on 
     // ei_printf("1: Pop Go right: %.5f\n", myBestClassificationValue);
    }
    
    if (myBestClassificationNumber == 2){   // 2water : go left
      digitalWrite(LEDG, LOW);              // Green LED on   
     // ei_printf("2: Water Go Left: %.5f\n", myBestClassificationValue);
    }
    
    if (myBestClassificationNumber == 3){   // 3fast got straight
      digitalWrite(LEDR, LOW);              // Red LED on            
     // ei_printf("3: Both Go Fast: %.5f\n", myBestClassificationValue);
    }

    // put more if statements here for your model

}
