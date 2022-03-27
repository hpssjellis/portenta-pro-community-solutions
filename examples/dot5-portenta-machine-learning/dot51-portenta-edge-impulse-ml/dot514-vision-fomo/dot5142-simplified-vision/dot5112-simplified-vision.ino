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
int myDelay = 0;  // delay between readings, can be zero, default 2000 = 2 seconds



/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Edge Impulse Inferencing Demo");

#ifdef EI_CAMERA_FRAME_BUFFER_SDRAM
    // initialise the SDRAM
    SDRAM.begin(SDRAM_START_ADDRESS);
#endif

    if (ei_camera_init()) {
        Serial.println("Failed to initialize Camera!");
    }
    else {
        Serial.println("Camera initialized");
    }

    for (size_t ix = 0; ix < ei_dsp_blocks_size; ix++) {
        ei_model_dsp_t block = ei_dsp_blocks[ix];
        if (block.extract_fn == &extract_image_features) {
            ei_dsp_config_image_t config = *((ei_dsp_config_image_t*)block.config);
            int16_t channel_count = strcmp(config.channels, "Grayscale") == 0 ? 1 : 3;
            if (channel_count == 3) {
                Serial.println("WARN: You've deployed a color model, but the Arduino Portenta H7 only has a monochrome image sensor. Set your DSP block to 'Grayscale' for best performance.");
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
    Serial.println("Starting inferencing in "+String(myDelay)+" microseconds...");

    // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
    if (ei_sleep(myDelay) != EI_IMPULSE_OK) {
        return;
    }

    Serial.println("Taking photo...\n");

    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_cutout_get_data;

    if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
        Serial.println("Failed to capture image\r\n");
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        Serial.println("ERR: Failed to run classifier, error number: " + String(err));
        return;
    }
    // print the predictions
    Serial.println("Predictions ");
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
    Serial.println("    anomaly score: " + String(result.anomaly, 5));
#endif
}
