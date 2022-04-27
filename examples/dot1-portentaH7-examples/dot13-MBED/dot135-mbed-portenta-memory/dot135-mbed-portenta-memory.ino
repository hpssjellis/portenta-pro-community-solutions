/*
* d-mbed-thread.ino
*
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
*
*
********************************************************** end ****************************************************
*
*/



#include "mbed.h"
#include "rtos.h"

using namespace mbed;  // sometimes needed
using namespace rtos;
#define MAX_THREAD_INFO 10

mbed_stats_heap_t heap_info;
mbed_stats_stack_t stack_info[ MAX_THREAD_INFO ];

Thread thread;

void myLedBlue_thread(){
   while (true) {
    digitalWrite(LEDB, !digitalRead(LEDB));   //switch on / off
    mbed_stats_heap_get(&heap_info);
    mbed_stats_stack_get(&stack_info[0]);
    mbed_stats_stack_get_each(stack_info, MAX_THREAD_INFO);

    //debug("\nThis message is from debug function");
    Serial.print("\nMemoryStats:");

    Serial.print("\n\tBytes allocated currently: "+String( heap_info.current_size));
    Serial.print("\n\tMax bytes allocated at a given time: "+String( heap_info.max_size));
    Serial.print("\n\tCumulative sum of bytes ever allocated: "+String( heap_info.total_size));
    Serial.print("\n\tCurrent number of bytes allocated for the heap: "+String( heap_info.reserved_size));
    Serial.print("\n\tCurrent number of allocations: "+String( heap_info.alloc_cnt));
    Serial.print("\n\tNumber of failed allocations: "+String( heap_info.alloc_fail_cnt));

    Serial.print("\nCumulative Stack Info:");
    Serial.print("\n\tMaximum number of bytes used on the stack: "+String( stack_info[0].max_size));
    Serial.print("\n\tCurrent number of bytes allocated for the stack: "+String( stack_info[0].reserved_size));
    Serial.print("\n\tNumber of stacks stats accumulated in the structure: "+String( stack_info[0].stack_cnt));

    Serial.print("\nThread Stack Info:");
    for (int i = 0; i < MAX_THREAD_INFO; i++) {
        if (stack_info[i].thread_id != 0) {
            Serial.print("\n\tThread: " + String(i) );
            Serial.print("\n\t\tThread Id: 0x"+String(  stack_info[i].thread_id) + "8lX");
            Serial.print("\n\t\tMaximum number of bytes used on the stack: "+String( stack_info[i].max_size));
            Serial.print("\n\t\tCurrent number of bytes allocated for the stack: "+String( stack_info[i].reserved_size));
            Serial.print("\n\t\tNumber of stacks stats accumulated in the structure: "+String( stack_info[i].stack_cnt));
        }
    }

      Serial.print(F("End of Data ------------------------"));
      
      ThisThread::sleep_for(15000);
      
   }
}

void setup() {
   pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   thread.start(myLedBlue_thread);
}

void loop() {
  // put your main code here, to run repeatedly:

}


