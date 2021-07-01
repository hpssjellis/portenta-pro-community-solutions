
/*================= Start Advanced Area ===============================*/

#include "tensorflow/lite/micro/all_ops_resolver.h"
//#include "tensorflow/lite/micro/kernels/micro_ops.h"
//#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"  // may have issues on M4 core
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "tensorflow/lite/c/common.h"
//#include "tensorflow/lite/c/c_api.h" // has interesting funcitons, not in main library


// Start Debug.cc information    ----------------------------------------------------------


 //  Note: CORE_CM7 deprecated as of MBED 1.3.0 , new names are:  PORTENTA_H7_M7,  ARDUINO_NANO33BLE
 #if  defined (CORE_CM7)   ||  defined (TEENSYDUINO) ||  defined (ARDUINO_NANO33BLE) ||  defined (YOUR_BOARD1) // CORE_CM7 is for the PortentaH7 outer core
 
    // do abosolutely nothing the default works

 // Note:  NANO_33_BLE deprecated as MBED of 1.30, new name: ARDUINO_NANO33BLE   
 #elif defined (CORE_CM4) ||   defined (NANO_33_BLE)   ||  defined (YOUR_BOARD2)  // CORE_CM4 is for the PortentaH7 inner core
 
     #define DEBUG_SERIAL_OBJECT (Serial) 

     extern "C" void DebugLog(const char* s) {
        static bool is_initialized = false;
        if (!is_initialized) {
           DEBUG_SERIAL_OBJECT.begin(9600);
           is_initialized = true;
        }
        DEBUG_SERIAL_OBJECT.print(s);
     }

 #elif defined (__SAM3X8E__)  ||  defined (YOUR_BOARD3) // Arduino UNO style boards

     #define DEBUG_SERIAL_OBJECT (SerialUSB) 

     extern "C" void DebugLog(const char* s) {
        static bool is_initialized = false;
        if (!is_initialized) {
           DEBUG_SERIAL_OBJECT.begin(9600);
           is_initialized = true;
        }
        DEBUG_SERIAL_OBJECT.print(s);
     }
 #elif defined (SEEED_XIAO_M0)  ||  defined (YOUR_BOARD4) // The new $5 USD Seeeduino XIAO board

     #define CFG_TUSB_DEBUG
     
 #else
   // don't do any debugging until you figure out your board  
   
   extern "C" void DebugLog(const char* s) {
     // Do not log debug info
   } 
 #endif


/*
 * nano_33_iot.build.board=SAMD_NANO_33_IOT  // have not got it working yet
 * 
*/

// End Debug.cc information    ----------------------------------------------------------


namespace { // Start namespace----------------------------------------------------------

   // define needed variables
   tflite::ErrorReporter* error_reporter = nullptr;
   const tflite::Model* model = nullptr;
   tflite::MicroInterpreter* interpreter = nullptr;
   TfLiteTensor* input = nullptr;
   TfLiteTensor* output = nullptr;
   // Create an area of memory to use for input, output, and intermediate arrays.
   constexpr int kTensorArenaSize = 2 * 1024;  // I like 20 * 1024;
   uint8_t tensor_arena[kTensorArenaSize];

}  // END namespace----------------------------------------------------------



/*================= End Advanced Area ===============================*/


/*================= Start Smart Area ================================*/


void modelSetup(const unsigned char theModel[]){

  // NOLINTNEXTLINE(runtime-global-variables)
  
   static tflite::MicroErrorReporter micro_error_reporter;
  
   error_reporter = &micro_error_reporter;  

   // Map the model into a usable data structure. This doesn't involve any
   // copying or parsing, it's a very lightweight operation.


   model = tflite::GetModel(theModel);  // name from the tflite converter model.h file

  
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)

  
  static tflite::AllOpsResolver resolver;


  /* // replace the above line if you know each operation to save space
  
  static tflite::MicroMutableOpResolver<5> micro_op_resolver;  // NOLINT
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_DEPTHWISE_CONV_2D,tflite::ops::micro::Register_DEPTHWISE_CONV_2D() );
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D,tflite::ops::micro::Register_MAX_POOL_2D() );
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,tflite::ops::micro::Register_CONV_2D() );
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED,tflite::ops::micro::Register_FULLY_CONNECTED() );
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,tflite::ops::micro::Register_SOFTMAX() );

   * 
   */
   

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
 
}


float modelPredict(float myInput){   // like tensorflowJS  const myPredictArray = await model.predict(xTrainingData).data()  
  
  input->data.f[0] = myInput;  // may have to edit this

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  
  // if (invoke_status != kTfLiteOk) {
  //    Serial.print("Error with input: " + String(myInput));
  // }

  return output->data.f[0];   // may have to edit this
}


/*================= End Smart Area ======================================*/

