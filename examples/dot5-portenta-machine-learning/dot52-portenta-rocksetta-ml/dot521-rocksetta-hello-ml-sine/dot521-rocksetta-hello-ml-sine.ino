/*
 * b01_makerML_hello_world.ino
 * simplifying TensorflowLite Micro Machine Learning for all makers
 * 
 * 
 * 
 * By Jeremy Ellis 
 * Twitter @rocksetta
 * Website https://rocksetta.com
 * created Aug 20th, 2020 
 * Github    https://github.com/hpssjellis/my-examples-for-the-arduino-portentaH7/tree/master/m09-Tensoflow
 *  
 *  
 *  Hashtags
 *  #TensorflowLiteMicro
 *  #TensorflowLiteForArduino
 *  #TinyMY
 *  #MakerML
 */





#include <Arduino.h> // Only needed by https://platformio.org/
#include <TensorFlowLite.h>
#include "model.h"
#include "rocksetta.h"


/*================= Start Maker Area ======================================*/


int myCounter = 0;
int myLed = LED_BUILTIN;   //or 7 or 5 or LED_BUILTIN or LEDB

void setup() {
  
  Serial.begin(9600);
  pinMode(myLed, OUTPUT);
  modelSetup(model_tflite);  // name of the model in the tab model.h
  
}


void loop() {
   myCounter +=1;
    if (myCounter >= 360){
        myCounter = 0;
    }
    
    float x_val = myCounter * 3.14/180;  // degrees to radians
    float y_val = sin(x_val);

    // This uses your model to make a prediction
    float predicted = modelPredict(x_val);

    Serial.println("sin(" +String(x_val)+ ") = " + String(y_val) + "\t predicted: " + String(predicted) );


    // y=1 LED is fully on. The LED's brightness can range from 0-255.
    int brightness = (int)(127.5f * (predicted+1));

     #if  defined (CORE_CM7)  ||  defined (CORE_CM4)

         if (brightness <= 128){
              digitalWrite(myLed, HIGH);  // means off
         } else {
               digitalWrite(myLed, LOW);  // means on             
         }
     #else
         analogWrite(myLed, brightness);  // not on Porttenta
     #endif 



    delay(3); // slows the process down a bit to see the sine wave

  
}


/*================= End Maker Area ======================================*/
