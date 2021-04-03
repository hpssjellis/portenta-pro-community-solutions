/*
 * 
 * DRAFT
 *  Edge Impulse model submitted by 
 *  15 y/o Dhruv Sheth
 */


#define EI_DSP_IMAGE_BUFFER_STATIC_SIZE 128


/* Includes ---------------------------------------------------------------- */
#include <anger-contempt-fear-happy-sadness-surprise_inference.h>

#include "camera.h"

CameraClass myCam;




unsigned long myTimer = millis();


// These variable must be global

unsigned long previousMillisR;
int ledStateR = HIGH;
int myAmountR;

unsigned long previousMillisG;
int ledStateG = HIGH;
int myAmountG;

unsigned long previousMillisB;
int ledStateB = HIGH;
int myAmountB;



// raw frame buffer from the camera
#define FRAME_BUFFER_COLS          320   // 160
#define FRAME_BUFFER_ROWS          240   // 120
//uint16_t frame_buffer[FRAME_BUFFER_COLS * FRAME_BUFFER_ROWS] = { 0 };

uint8_t frame_buffer[320*240] __attribute__((aligned(32)));

// cutout that we want (this does not do a resize, which would also be an option, but you'll need some resize lib for that)
#define CUTOUT_COLS                 EI_CLASSIFIER_INPUT_WIDTH
#define CUTOUT_ROWS                 EI_CLASSIFIER_INPUT_HEIGHT
const int cutout_row_start = (FRAME_BUFFER_ROWS - CUTOUT_ROWS) / 2;
const int cutout_col_start = (FRAME_BUFFER_COLS - CUTOUT_COLS) / 2;

/**
 * This function is called by the classifier to get data
 * We don't want to have a separate copy of the cutout here, so we'll read from the frame buffer dynamically
 */
int cutout_get_data(size_t offset, size_t length, float *out_ptr) {
    // so offset and length naturally operate on the *cutout*, so we need to cut it out from the real framebuffer
    size_t bytes_left = length;
    size_t out_ptr_ix = 0;

    // read byte for byte
    while (bytes_left != 0) {
        // find location of the byte in the cutout
        size_t cutout_row = floor(offset / CUTOUT_COLS);
        size_t cutout_col = offset - (cutout_row * CUTOUT_COLS);

        // then read the value from the real frame buffer
        size_t frame_buffer_row = cutout_row + cutout_row_start;
        size_t frame_buffer_col = cutout_col + cutout_col_start;

        // grab the value and convert to r/g/b
        uint8_t pixel = frame_buffer[(frame_buffer_row * FRAME_BUFFER_COLS) + frame_buffer_col];


        //uint8_t pixel = (pixelTemp>>8) | (pixelTemp<<8);
        //uint8_t pixel = 255-pixelTemp;
        
        uint8_t r = pixel;
        uint8_t g = pixel;
        uint8_t b = pixel;

        // then convert to out_ptr format
        float pixel_f = (r << 16) + (g << 8) + b;
        out_ptr[out_ptr_ix] = pixel_f;

        // and go to the next pixel
        out_ptr_ix++;
        offset++;
        bytes_left--;
    }

    // and done!
    return 0;
}






/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
     // Serial.begin(921600);
  pinMode(LEDB, OUTPUT); 
  pinMode(LEDG, OUTPUT);   // same as LED_BUILTIN
  pinMode(LEDR, OUTPUT); 

  // Init the cam
  myCam.begin(CAMERA_R320x240, 30);

  // Skip 60 frames
 // myCam.skip_frames(frame_buffer, 30);

    Serial.println("Edge Impulse Inferencing Demo");
}

/**
 * @brief      Arduino main function
 */
void loop()
{
    ei_printf("Edge Impulse standalone inferencing (Arduino)\n");



    ei_impulse_result_t result = { 0 };


     // if (Serial) {
    // Grab frame and write to serial
   // if (cam.grab(frame_buffer) == 0) {
    //  Serial.write(frame_buffer, 320*240);
   // }
  //}
     // potentially need to check for above framebuffer == 0
     
   int myCamResult =  myCam.grab(frame_buffer); // myCamResult should be zero 

  //  int myCamResult;
  //  do {
  //      myCamResult =  myCam.grab(frame_buffer);
   // } while (myCamResult != 0);
  
 // myCam.skip_frames(frame_buffer, 60);

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = CUTOUT_COLS * CUTOUT_ROWS;
    features_signal.get_data = &cutout_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
    ei_printf("run_classifier returned: %d\n", res);

    if (res != 0) return;

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    ei_printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            ei_printf(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("%.3f", result.anomaly);
#endif
    ei_printf("]\n");

// turn off all Portenta LED's
     digitalWrite(LEDR, HIGH);
     digitalWrite(LEDG, HIGH);
     digitalWrite(LEDB, HIGH);


    // human-readable predictions
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);

    //  if (result.classification[ix].value >= 0.7){
    //     digitalWrite(LEDB, LOW);
    //  }

    //   else if (result.classification[ix].value >= 0.5 && result.classification[ix].value < 0.7){
    //     digitalWrite(LEDG, LOW);
    //  }
      
    //  else if (result.classification[ix].value >= 0.3 && result.classification[ix].value < 0.5){
     //    digitalWrite(LEDR, LOW);
    //  }
      

        
    }   // end main classification loop
    if (result.classification[0].value>= 0.8){  // anger
       LED_onboard_setRGB(255, 25, 25);         //mainly red
    }

    if (result.classification[1].value>= 0.8){  // contempt
       LED_onboard_setRGB(200, 35, 35);         // pinkish
    }

    if (result.classification[2].value>= 0.8){  // fear
       LED_onboard_setRGB(255, 255, 25);        //red-green
    }
    if (result.classification[3].value>= 0.8){  // happy
       LED_onboard_setRGB(25, 25, 255);         // blue
    }

    if (result.classification[4].value>= 0.8){  // sad
       LED_onboard_setRGB(25, 255, 25);          //green
    }
    
    if (result.classification[5].value>= 0.8){  // surprise
       LED_onboard_setRGB(255, 255, 255);       // white
    }
                        
    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif



/*   unbracket to grab an image of what the board sees   */

/*
    for (size_t ix = 0; ix < features_signal.total_length; ix++) {
        float value[1];
        features_signal.get_data(ix, 1, value);

        ei_printf("0x%06x", (int)value[0]);
        if (ix != features_signal.total_length - 1) {
          ei_printf(", ");
        }
    }

*/

    

    Serial.println();
    delay(2000);
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


 // unsigned long myNow = millis();
 //  LED_onboard_setRGB(25, 25, 255); // example blue

 // if (myNow - myTimer >= 1000){   // color change delay 1000 = 1 second
  //   myTimer = myNow;
 //    myAmountR = 2;  //rand()% 256;
 //    myAmountG = 2;  //rand()% 256;
 //    myAmountB =  rand()% 256;   
  //   Serial.println("Red:"+String(myAmountR) + ", Green:"+ String(myAmountG) + ", Blue:"+ String(myAmountB));
 // }


 
 // LED_onboard_setRGB( myAmountR, myAmountG, myAmountB);

    
}   // end main loop




// helper utilities to make onboard LED's 
// act like they can do PWM
// I should use a thread, but too lazy

void LED_onboard_setRGB(int colorR, int colorG, int colorB){

   LED_onboard_analogWrite(LEDR, colorR, previousMillisR, ledStateR);
   LED_onboard_analogWrite(LEDG, colorG, previousMillisG, ledStateG);
   LED_onboard_analogWrite(LEDB, colorB, previousMillisB, ledStateB);
}

void LED_onboard_analogWrite(int myPin, int myPWM, unsigned long &myMillis, int &myState){
  unsigned long currentMillis = micros();
  
  if (myPWM > 255){myPWM = 255;}
  if (myPWM < 0){myPWM = 0;}
  
  if (myState == LOW){      
     if(currentMillis - myMillis >= myPWM){
        myState = HIGH;
        digitalWrite(myPin, myState);  
        myMillis = currentMillis;
      }     
  } else { 
     if(currentMillis - myMillis >= 255-myPWM){
        myState = LOW;
        digitalWrite(myPin, myState);    
        myMillis = currentMillis;  
     }
  } 
}




