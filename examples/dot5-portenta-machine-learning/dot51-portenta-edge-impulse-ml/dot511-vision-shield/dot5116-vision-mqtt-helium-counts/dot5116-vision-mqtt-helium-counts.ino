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
 */
/* Includes ---------------------------------------------------------------- */
#include <ei-v20unknown-1popGoRight-2waterGoLeft-3fast-v2-0-0_inferencing.h>
#include "edge-impulse-advanced.h"

#include <MKRWAN.h>
#include <CayenneLPP.h>



LoRaModem modem;
CayenneLPP lpp(51);
bool connected = false;
bool myWaitForDownlink = false;
bool myDownLink = false;

unsigned long myStoredMillisA;
unsigned long myStoredMillisB;
const unsigned long myTimerDurationA = 30000;  // delay between sending data
const unsigned long myTimerDurationB = 5000;   // delay to wait for a downlink

int myLppCount = 0;

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// Note: Best to have the App_Device hard coded. Run the program once to see the value.
//#include "arduino_secrets.h"

#define SECRET_APP_EUI "0000000000000000" 
#define SECRET_APP_KEY "00000000000000000000000000000000"


String appEui = SECRET_APP_EUI;   // just strings of the above 
String appKey = SECRET_APP_KEY;





// Global Variables
int myDelay = 500;  // delay between readings, can be zero, default 2000 = 2 seconds
int myClassCount[EI_CLASSIFIER_LABEL_COUNT]; // array to store all counts



/**
* @brief      Arduino setup function
*/
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Edge Impulse Inferencing Demo");

    pinMode(LEDR, OUTPUT); 
    pinMode(LEDG, OUTPUT);   // this is LED_BUILTIN
    pinMode(LEDB, OUTPUT); 

     Serial.println("Wait 4");
  delay(3000);             // delay instead, so it works when disconnected
  digitalWrite(LEDG, HIGH);// allows time to connect serial monitor

  Serial.println("Wait 3");
  delay(3000);       
  digitalWrite(LEDG, LOW);
 
  Serial.println("Wait 2");
  delay(3000);       
  digitalWrite(LEDG, HIGH);

  Serial.println("Wait 1");
  delay(3000);       
  digitalWrite(LEDG, LOW);


  
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
 
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  Serial.println("Now Disabling all channels and enable channel 1 only for Helium ");

  modem.disableChannel(0);
  modem.enableChannel(1);    // only one enabled for Helium
  modem.disableChannel(2);
  modem.disableChannel(3);
  modem.disableChannel(4);
  modem.disableChannel(5);
  modem.disableChannel(6);
  delay(5000);
  Serial.println("Now Joining the Helium Network ");


    
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
void loop(){


while (!connected) {
    Serial.println("trying to reconnect");
    digitalWrite(LEDR, HIGH); // new boards HIGH = off
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    connected = modem.joinOTAA(appEui, appKey);
    delay(5000);    
    digitalWrite(LEDR, HIGH); // new boards HIGH = off
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    delay(1000);
  }

   if (myWaitForDownlink){
    
      char rcv[64];
      int i = 0;
      while (modem.available()) {
        rcv[i++] = (char)modem.read();
        myDownLink = true;
      }

       if (millis() - myStoredMillisB >= myTimerDurationB){  // Test whether the period has elapsed
         myStoredMillisB = millis();  
         if (!modem.available()) {
             Serial.println("No downlink message received at this time.");
             myWaitForDownlink = false;
         }
      }
      

      if (myDownLink) {
        myWaitForDownlink = false;
        myDownLink = false;
        Serial.print("Received: ");
        for (unsigned int j = 0; j < i; j++) {
          Serial.print(rcv[j] >> 4, HEX);
          Serial.print(rcv[j] & 0xF, HEX);
          Serial.print(" ");
        }
        Serial.println();
        digitalWrite(LEDR, LOW); // new boards HIGH = off
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, LOW);

      }
         
    }


  if (millis() - myStoredMillisA >= myTimerDurationA){  // Test whether the period has elapsed
    myStoredMillisA = millis();                         // IMPORTANT to save the next stored time 


    
      //lpp.reset();
      //float x = rand() / 10000000.0; //analogRead(A0)
      //lpp.addTemperature(1, x); 
  
      
      
      /*
      // Can do any of these
      lpp.reset();
      lpp.addDigitalInput(1, 0);
      lpp.addDigitalOutput(2, 1);
      lpp.addAnalogInput(3, 1.23f);
      lpp.addAnalogOutput(4, 3.45f);
      lpp.addLuminosity(5, 20304);
      lpp.addPresence(6, 1);
      lpp.addTemperature(7, 26.5f);
      lpp.addRelativeHumidity(8, 86.6f);
      lpp.addAccelerometer(9, 1.234f, -1.234f, 0.567f);
      lpp.addBarometricPressure(10, 1023.4f);
      lpp.addGyrometer(1, -12.34f, 45.56f, 89.01f);
      lpp.addGPS(1, -12.34f, 45.56f, 9.01f);
    
      lpp.addUnixTime(1, 135005160);
      
      lpp.addGenericSensor(1, 4294967295);
      lpp.addVoltage(1, 3.35);
      lpp.addCurrent(1, 0.321);
      lpp.addFrequency(1, 50);
      lpp.addPercentage(1, 100);
      lpp.addAltitude(1 , 50);
      lpp.addPower(1 , 50000);
      lpp.addDistance(1 , 10.555);
      lpp.addEnergy(1 , 19.055);
      lpp.addDirection(1 , 90);
      lpp.addSwitch(1 , 0);
      
      lpp.addConcentration(1 , 512);
      lpp.addColour(1 , 64, 128, 255);
      
      */
    
      
    //  Serial.println();
      Serial.println("Sending: frequencies");

      lpp.reset();
      myLppCount += 1;
      if (myLppCount > 2){myLppCount = 0;} 
      lpp.addFrequency(myLppCount+4 , (int)myClassCount[myLppCount]);  // 0 unknown
     // lpp.addFrequency(7, (int)myClassCount[1]);  // 1 pop
     // lpp.addFrequency(8, (int)myClassCount[2]);  // 2 water 

      
      int err;
      modem.beginPacket();
      modem.write(lpp.getBuffer(), lpp.getSize());
      err = modem.endPacket(true);






      
      if (err > 0) {
        Serial.println("Message sent correctly!");
        digitalWrite(LEDR, HIGH); // new boards HIGH = off
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, LOW);
      } else {
        Serial.println("Error sending message :(");
        Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
        Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
        digitalWrite(LEDR, LOW); // new boards HIGH = off
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDB, HIGH);
      }
      myStoredMillisB = millis();
      myWaitForDownlink = true;
      //delay(1000);







     // delay(30000);  // delay 30 seconds for testing
     
  }  // end timerA





  
    Serial.println("Starting inferencing in "+String(myDelay)+" microseconds...");

    // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
    if (ei_sleep(myDelay) != EI_IMPULSE_OK) {
        return;
    }


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


    int myBestClassificationNumber = -1;  
    float myBestClassificationValue = 0.25;   // lowest best allowable value
    
    
    // print the predictions
   // Serial.println("Predictions ");
    // For complex prints best to run Edge Impulse ei_printf
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \nTotals:");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%s: %d\n", result.classification[ix].label, myClassCount[ix]);

        if (result.classification[ix].value > myBestClassificationValue ){
           myBestClassificationNumber = ix;                      // find the biggest array value
           myBestClassificationValue = result.classification[ix].value;  
        }

    }        
    ei_printf(": \n");
    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    Serial.println("    anomaly score: " + String(result.anomaly, 5));
#endif


    digitalWrite(LEDB, HIGH);   //on board LED's are turned off by HIGH    
    digitalWrite(LEDG, HIGH);   
    digitalWrite(LEDR, HIGH); 

    // I find it less confusing if the 0unknown does nothing
    if (myBestClassificationNumber == 0){    // 0 unknown do nothing
        myClassCount[0] += 1;
        digitalWrite(LEDB, LOW);    
        digitalWrite(LEDG, LOW);   
        ei_printf("0: Unknown: %.5f, total: %d\n", myBestClassificationValue, myClassCount[0]);
    }
 

    if (myBestClassificationNumber == 1){   // 1pop: Go Right
      myClassCount[1] += 1;
      digitalWrite(LEDB, LOW);              // Blue LED on
      ei_printf("1: Pop Go right: %.5f, total: %d\n", myBestClassificationValue, myClassCount[1]);
    }
    
    if (myBestClassificationNumber == 2){   // 2water : go left
      myClassCount[2] += 1;
      digitalWrite(LEDG, LOW);              // Green LED on  
      ei_printf("2: Water Go Left: %.5f, total: %d\n", myBestClassificationValue, myClassCount[2]);
    }
    
    if (myBestClassificationNumber == 3){   // 3fast got straight
      myClassCount[3] += 1;
      digitalWrite(LEDR, LOW);              // Red LED on         
      ei_printf("3: Both Go Fast: %.5f, total: %d\n", myBestClassificationValue, myClassCount[3]);
    }

    // put more if statements here for your model

    Serial.println();
}
