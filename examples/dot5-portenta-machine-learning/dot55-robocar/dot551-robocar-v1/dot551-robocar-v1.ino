

/*
 * Base RC Robocar FOMO 96x96 with DC motor, servo and OLED all on the M7 core
 * 
 * Build instructions will eventually be ready at 
 * https://github.com/hpssjellis/robocar
 *
 * Code can be run without any attachments except the Portenta H7 Vision Shield and the serial monitor will work
 * with the servo and Big Motor sdriver the car will work
 * With the grayscale OLED it can work for debugging without the car.
 * 
 * line 141 in the include file "edge-impulse-advanced-v2.h" I changed camera speed from 30 to 60 frames per second
 * Seems to help
 * 
 * 
 * 
 * Use at your own risk!
 * MIT license
 * by Jeremy Ellis Twitter @rocksetta
 * 
 * Basic Pins
 * // mainly at line 277
 * 
 *   pinMode(D3, OUTPUT);   // digital 0 to 1 Forward
 *   pinMode(D1, OUTPUT);   // digital 0 to 1 reverse does not have to be connected
 *   pinMode(D5, OUTPUT);   // PWM 0 to 255 speed
 *   
 * D5  PWM motor
 * D2  servo
 * D1  Direction backwards
 * D3  Direction forwards
 * neither D1,D3 = glide
 * both D1,D3 = Stop probably a good idea to set D5 to 0
 * 
 * 
 *  FOR the GRAYSCALE Waveshare OLED 128 x 128 using library Adafruit_SSD1327.h
 *   
 *   blue  DIN (mosi) D8
 *   yellow (sck) D9                                                                                                                                                             
 *   orange (cs) D7
 *   green (dc)  D6
 *   white (reset) not needed but D14 if you did
 *
 * another reference here 
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
 *
 *
 *
 * Note: You can change how many objects edgeimpulse FOMO detects by editing the model_metadata.h file.
 * which is located in the arduino installed library at 
 * src\model-parameters\model_metadata.h
 * I changed the following line form 10 to 3
 * #define EI_CLASSIFIER_OBJECT_DETECTION_COUNT       3
 * Although finding that it is not relaly needed
 * 
 * Through out the code look (search) for the word "marker" which shows important areas
 * 
 */




 
/* Includes ---------------------------------------------------------------- */

// marker 1: This is the file from EdgeImpulse.com the arduino model download
#include <ei-v25-fomo-eyes-shades_inferencing.h>

//#include <ei-fomo-v21-just-eye_inferencing.h>

// marker 2: comment the next line of code out if you don't want to use the 128x128 grayscale OLED
// Makes your 200ms code ~65 ms faster.

  #define USE-GRAY-OLED




// The following code block will not compile if the above is commented out
//#ifdef USE-GRAY-OLED
//#endif

// marker 3: I have condensed the edgeimpulse library of code we don't need to worry about.
#include "edge-impulse-advanced-v2.h"

#include <Servo.h>
#include "mbed.h"
#include "rtos.h"
//using namespace mbed;  // sometimes needed
using namespace rtos;


#ifdef USE-GRAY-OLED

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

#endif


// Global Variables
int myDelay = 0;  // delay between readings, can be zero, default 2000 = 2 seconds
int x1Map, x2Map, y1Map, y2Map;


// the OLED uses these
#define CUTOUT_COLS                 EI_CLASSIFIER_INPUT_WIDTH
#define CUTOUT_ROWS                 EI_CLASSIFIER_INPUT_HEIGHT
const int cutout_row_start = (EI_CAMERA_RAW_FRAME_BUFFER_ROWS - CUTOUT_ROWS) / 2;
const int cutout_col_start = (EI_CAMERA_RAW_FRAME_BUFFER_COLS - CUTOUT_COLS) / 2;


// main thread for the servo and dc motor to keep their operation away from the analysis
Thread myThread01;

unsigned long myStart; 
const int  MY_THREAD_SLEEP = 10;      // default 10, ms the motor thread sleeps, lower the number faster the response

int myGlobalD5 = 0; // the Big Motor PWM speed

Servo myServo_D2;


int myTurning = 0;
int myServoAngle = 0;


// might still use
//const float MY_FOMO_CUTOFF = 0.85;    // default 0.85;

// marker 4:  most of the main constants and variables
const int MY_MIDDLE_X = 48; //38;  //42;    //  (98 pixels / 2) - 4

const int MY_SERVO_MIN = 60;        // degrees
const int MY_SERVO_SLIGHT_LOW = 80;
const int MY_SERVO_STRAIGHT = 90;
const int MY_SERVO_SLIGHT_HIGH = 100;
const int MY_SERVO_MAX = 120;

const int MY_MEASURE_MIN = -30;   // -20 left flatter turning, -40 sharper turning
const int MY_MEASURE_MAX =  30;   // 20 right flatter turning, 40 sharper turning


// Stops all sharp turns from random errors.
//int myServoChangeAmount = 20;  //  default 2 to 10 degrees. how much to change the turn angle if a change is needed.
int myServoNow = MY_SERVO_STRAIGHT;    // start going straight


const int MY_PWM_MIN = 33;      // 25
const int MY_PWM_SLOW = 34;     // 25
const int MY_PWM_MEDIUM = 38;   // 25
const int MY_PWM_MAX = 45;      // 50;  // careful this is the max speed for the car and can be up to 255!
int myPwmNow = 0;                 // start with it stopped!

int myGlobalCount; // countof all relevant objects


// marker 5: The main Thread to change the motor and servo away from the other code
void myLedBlue_myThread01(){
  // don't do display or ei_printf or Serial.println() commands here as it goes too fast
   while (true) {

      // PROTECT PWM FROM GOING TOO FAST
      if (myGlobalD5 > MY_PWM_MAX) {myGlobalD5 = MY_PWM_MAX; }    
      analogWrite(D5, myGlobalD5); 
      
      // PROTECT THE SERVO FROM WEIRD VALUES
      if (myServoNow > MY_SERVO_MAX) {myServoNow = MY_SERVO_MAX;} 
      if (myServoNow < MY_SERVO_MIN) {myServoNow = MY_SERVO_MIN;} 
      myServo_D2.write(myServoNow);  
      
      ThisThread::sleep_for(MY_THREAD_SLEEP);   
   }
   
}


/**
* @brief      Arduino setup function
*/
void setup(){
   
     myThread01.start(myLedBlue_myThread01);
    // put your setup code here, to run once:
    Serial.begin(115200);
   // Serial.println("Edge Impulse Inferencing Demo");

    pinMode(LEDR, OUTPUT); 
    pinMode(LEDG, OUTPUT);   // this is LED_BUILTIN
    pinMode(LEDB, OUTPUT); 

    myServo_D2.attach(D2);   // D2 should do PWM on Portenta
    

    pinMode(D3, OUTPUT);   // digital 0 to 1 
    pinMode(D1, OUTPUT);   // digital 0 to 1 
    pinMode(D5, OUTPUT);   // PWM 0 to 255

    digitalWrite(D1, 0);    // set one direction   
    digitalWrite(D3, 1);       // set one direction 
   
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
    
    // Following for the Grayscale OLED
#ifdef USE-GRAY-OLED

   if ( ! display.begin(0x3D) ) {   // start Grayscale OLED
     Serial.println("Unable to initialize OLED");
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

#endif
    
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop(){
  
    myStart = millis();  // milliseconds since the Portenta started

    // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
    if (ei_sleep(myDelay) != EI_IMPULSE_OK) {
        return;
    }
    
   // Serial.println("Starting inferencing in "+String(myDelay)+" microseconds...");

#ifdef USE-GRAY-OLED

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

    display.drawRect(0,0,128,128, SSD1327_WHITE );  // rectangle around outside of OLED

#endif

    myGlobalCount = 0;  // how many acceptable objects
   // myPwmOld = myPwmNow; 
    myPwmNow = 0;   //  iF no objects we want the car to stay stopped


    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_cutout_get_data;

    if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
        Serial.println("Failed to capture image\r\n");
        return;
    }

    // marker 6: This Runs the machine ledarning pre-trained classifier
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        Serial.println("ERR: Failed to run classifier, error number: " + String(err));
        return;
    }

      int myBbxTotal = 0; // sums the thing we are interested in
      int myDivider = 0;
      int myBbxAverage = 0;
      int myBbxMax = 0; // to determin how close the x locations are
      
      int my0 = 0;  // for background info
      int my9 = 0;
      
       
    bool bb_found = result.bounding_boxes[0].value > 0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++) {
    
       auto bb = result.bounding_boxes[ix];  // auto is a cool variable that changes it's type as needed

      
      if (bb.value == 0){     // how many classifications came up with nothing
         my0 += 1;
      }


     // marker 7: The more objects you detect the bigger chance for false detections
     // here is where you identify those objects. I put numbers in front of the label name to make this easier
    
      if (String(bb.label).substring(0, 1) == "1"){  // eye detection
         myBbxTotal += (int)bb.x;
         if (myBbxMax < (int)bb.x ) {myBbxMax = (int)bb.x;}
         myDivider += 1;
      }

      if (String(bb.label).substring(0, 1) == "9"){  // if I make just a background label
         my9 += 1;
      }

     

        int xMap = map(bb.x, 0,96, 0,127);
        int yMap = map(bb.y, 0,96, 0,127);
        int widthMap = map(bb.width, 0,96, 0,127);
        int heightMap = map(bb.height, 0,96, 0,127);
#ifdef USE-GRAY-OLED       
        display.setCursor(xMap+2, yMap);
        display.println(bb.label);

        // draw the box where the FOMO was found
        display.drawRect(xMap, yMap, widthMap, heightMap, SSD1327_WHITE ); // good value`               

        display.setCursor(3, 30);  // write them near the left midway down the page
#endif

        
    }        // END of analysis loop!

    if (!bb_found) {
        ei_printf("    No objects found. ");
    }



    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    Serial.println("    anomaly score: " + String(result.anomaly, 5));
#endif
   


//--------------------------- start new stuff----------------------

       // mark 8: This is the important student area where we do stuff to drive the car.

        if (my9 > 0) {
          // background found just ignore    
        }

       
        if (myBbxTotal > 0){  // saw at least one eye
          myBbxAverage = myBbxTotal / myDivider;
          
          
#ifdef USE-GRAY-OLED          
          display.println("1 bigeye Fast"); 
#endif    
        
          digitalWrite(D1, 0);   // zero forward, both break
          digitalWrite(D3, 1);    // 1 forward,    neither glide  
 
          myTurning = (int)myBbxAverage - MY_MIDDLE_X;    // 48 for middle location may change
        
         
          // make the angles bigger by making the mapping smaller to offset averaging many X values
          myServoAngle = map(myTurning, MY_MEASURE_MIN,MY_MEASURE_MAX, MY_SERVO_MIN, MY_SERVO_MAX); // raw position to car turn angle




        // marker 9: The following code is my attempt to change the speed of the car
        // It is not needed you could just have the car go one speed
        //  myPwmNow = MY_PWM_MIN;


////////////////////////////////////////////////////////////////////////////////////////


         // If the angle is near the center do these else go minimum speed
         if (myServoAngle > MY_SERVO_SLIGHT_LOW && myServoAngle < MY_SERVO_SLIGHT_HIGH){

           // Set LED's to be blue // showing near the center average
           digitalWrite(LEDR, HIGH);    
           digitalWrite(LEDG, HIGH);     
           digitalWrite(LEDB, LOW); 
                     
           if (myDivider >= 3 ){ // more than or equal to 3 objects go fast!        
              myPwmNow = MY_PWM_MAX;  //go fast 
              myServoAngle = MY_SERVO_STRAIGHT; // go straight if fast
           } 
           // If the angle is near the center and 2  are detected then go middle speed. 
           if ( myDivider == 2 ){
              myPwmNow = MY_PWM_MEDIUM;  // Go medium speed
           }         
           // If the angle is near the center and 1 object then go slow.
           if (myDivider == 1 ){
               myPwmNow = MY_PWM_SLOW;  //go kind of slow   
           }
         } else {  // else objects not near the center so go slow
            
           // Set LED's to be green // showing big turning so go slow
           digitalWrite(LEDR, HIGH);    
           digitalWrite(LEDG, LOW);     
           digitalWrite(LEDB, HIGH);
           myPwmNow = MY_PWM_MIN;  //go really slow       
        }

        // end of marker 9:
///////////////////////////////////////////////////////////////////////////////////////
         

        // this check is now not really needed
        if (myPwmNow > MY_PWM_MAX) {myPwmNow = MY_PWM_MAX;}

        // variable sent to the turning thread
        myServoNow = myServoAngle;

   
        } else {   //No objects detected so stop the car!
          digitalWrite(D1, 0);   // zero forward, both break
          digitalWrite(D3, 0);    // 1 forward,    neither glide
#ifdef USE-GRAY-OLED         
          display.setCursor(3, 30);
          display.println("0: I am lost");    
#endif
          myPwmNow = 0;   // stop if nothing seen
         
          // myServoNow = MY_SERVO_STRAIGHT;  // better to not reset the direction
          
          // Since car is stopped turn off all LED's 
          digitalWrite(LEDB, HIGH);   //on board LED's are turned off by HIGH    
          digitalWrite(LEDG, HIGH);   
          digitalWrite(LEDR, HIGH);    
        
       }


//-------------------------------- end new stuff -------------------------------
      
       // marker 10: This area is just printing the information to the serial monitor
   
       Serial.print(F("Time(ms):"));
       int myFomoTime = millis() - myStart;
       Serial.print(myFomoTime);


       Serial.print(F(", myTurn:"));
       Serial.print(myTurning);
       Serial.print(F(", servo:"));
       Serial.print(myServoNow);
       Serial.print(F(", PWM:"));
       Serial.print(myPwmNow);

       
       Serial.print(F(", myBbxTotal: "));
       Serial.print(myBbxTotal);
       Serial.print(F(", myDivider: "));
       Serial.print(myDivider);
       Serial.print(F(", my0: "));
       Serial.print(my0);
       Serial.print(F(", my9: "));
       Serial.println(my9);
       
       // marker 10: This area is printing the information to the grayscale OLED

#ifdef USE-GRAY-OLED
      display.setCursor(20,10);
      display.println("Rocksetta-Drive");  
       
      display.setCursor(10,120);
      display.println("Servo:");
      display.setCursor(50, 120);
      display.println(String(myServoNow));
      display.setCursor(70, 120);
      display.println("PWM:");
      display.setCursor(100, 120);
      display.println(String(myPwmNow));
      
     // Last thing is to show the 128x128 GRAYSCALE OLED
     display.display();  // OLED 4 bit 16 color GRAYSCALE update
#endif

      myGlobalD5 = myPwmNow;  // set the motor speed so that the thread that changes the motor speed knows what to change it to.
         
}


 
