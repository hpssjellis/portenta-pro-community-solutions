/* Edge Impulse Arduino examples
 * Copyright (c) 2020 EdgeImpulse Inc.
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
/* Both these libraries need to be installed:------------------------------- */
/* w6-words_inference.zip is in the zippedLibraries part of this library in your arduino/libraries/Portenta-pro-community-solution folder */
#include <w6-words_inference.h>


/* Arduino_LSM9DS1 is a standard Arduino library that you should be able to search for- */
#include <Arduino_LSM9DS1.h>
// #include <MKRIMU.h>   // If using the MKRIMU with the portenta bend out the RESET pin and use this library include file


/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2    9.80665f
String myWords ="";
/* Private variables ------------------------------------------------------- */
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal

/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(10000);
    Serial.println("Edge Impulse Inferencing Demo");

    if (!IMU.begin()) {
        ei_printf("Failed to initialize IMU!\r\n");
    }
    else {
        ei_printf("IMU initialized\r\n");
    }

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 3 (the 3 sensor axes)\n");
        return;
    }
}

/**
* @brief      Printf function uses vsnprintf and output using Arduino Serial
*
* @param[in]  format     Variable argument list
*/
void ei_printf(const char *format, ...) {
   static char print_buf[1024] = { 0 };

   va_list args;
   va_start(args, format);
   int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
   va_end(args);

   if (r > 0) {
       Serial.write(print_buf);
   }
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop()
{
   // ei_printf("\nStarting inferencing in 2 seconds...\n");

    digitalWrite(LEDB, HIGH);  //off
    delay(2000);            
    digitalWrite(LEDB, LOW);  // on  
    delay(50);    
    digitalWrite(LEDB, HIGH);  
    delay(50);           
    digitalWrite(LEDB, LOW);
  //  ei_printf("Sampling...\n");

    // Allocate a buffer here for the values we'll read from the IMU
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        // Determine the next tick (and then sleep later)
        uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

        IMU.readAcceleration(buffer[ix], buffer[ix + 1], buffer[ix + 2]);

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        delayMicroseconds(next_tick - micros());
    }

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
        return;
    }

    // print the predictions
   // ei_printf("Predictions ");
   // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
    //    result.timing.dsp, result.timing.classification, result.timing.anomaly);
   // ei_printf(": \n");

   
    String tempLetter = "";
    int bestPercent = 0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    //    ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);


   // important that S is the 4th  (3) element
  // hack for "S" being badly trained grabs S even if other label in loop and stops loop
 // if (result.classification[3].label == "S" && result.classification[3].value > 0.2){
  //       myWords += "S";
  //      // Serial.print(myWords + ": "+ String(result.classification[3].value*100) + "%, note ix: "+ ix);
  //       Serial.println("S: "+ String(result.classification[3].value*100) + "%, ");
  //       break; //to finish this loop
  //  } else  {

      if (result.classification[ix].value > 0.05){ 
    

        if (result.classification[ix].label == "erase" && result.classification[6].value < 0.2){   // only if "space" less than 50%
               myWords.remove(myWords.length()-1); 
              // Serial.println("Deleting one letter:"+ String(result.classification[ix].value*100)+"%, ix: "+ ix );
               Serial.println("Deleting one letter:"+ String(result.classification[ix].value*100)+"%, ");
              // Serial.println(myWords);
        }
        if (result.classification[ix].label == "unknown"){
              //Serial.println("Clear All: "+ String(result.classification[ix].value*100)+"%, ix: "+ ix );
              Serial.println("Clear All: "+ String(result.classification[ix].value*100)+"%, " );
              myWords = "";
        }
        if (result.classification[ix].label == "space" && result.classification[5].value < 0.2){ // only if erase less than 50%
             // Serial.println("Adding a space: "+ String(result.classification[ix].value*100)+"%, ix: "+ ix);
              Serial.println("Adding a space: "+ String(result.classification[ix].value*100)+"%, ");
              myWords += " ";   // add a space
             // Serial.println(myWords + ".");
        }        
        if (result.classification[ix].label == "still"){  // do nothing
              //Serial.println(myWords + ".");
        }
        const char* L = result.classification[ix].label;  // L for letter
        if (L == "W" || L == "O" || L == "R" || L == "D" || L == "S"){  
            int myPercent = result.classification[ix].value * 100;
            if ( myPercent > bestPercent){
              bestPercent = myPercent;
              tempLetter = String(L);
              //Serial.print(tempLetter + ": "+ String(myPercent) + "%, ix: "+ ix +", ");
              Serial.print(tempLetter + ": "+ String(myPercent) + "%, ");
            }
        }

      } // if over 0.25 
   // } // end little "S" hack 
    
    } 
    if ( bestPercent > 0){   
       Serial.println(); 
    }
    myWords += tempLetter; //update myWords with best estimate

    Serial.println(myWords + ".");


}

    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif


#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_ACCELEROMETER
#error "Invalid model for current sensor"
#endif
