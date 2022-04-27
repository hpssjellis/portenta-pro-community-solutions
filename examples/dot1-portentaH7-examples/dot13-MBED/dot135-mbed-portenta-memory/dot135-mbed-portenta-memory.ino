/*
* dot135-mbed-portenta-memory.ino
* Trying to hack together something that shows information about the Portenta Memory use.
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




// Global Variables

long myDelay  = 8000;    // how long to wait for tons of data  8000 = 8 seconds

mbed_stats_heap_t heap_info;
mbed_stats_stack_t stack_info[ MAX_THREAD_INFO ];

Thread myThread;


// Helper funcitons

#include <malloc.h>
#include "mbed_mem_trace.h"


extern char* _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *)0x8040000;   // Size + 0xc0000
char *ramend = (char *)0x804c000;     // This is totally wrong


void print_memory_info() {
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));
 
    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
         Serial.println("Thread: 0x" + String( stats[i].thread_id)+"X, Stack size: " + String( stats[i].max_size) + " / "+ String( stats[i].reserved_size) );
    }
    free(stats);
 
    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
     Serial.println("Heap size: "+ String( heap_stats.current_size) + " / " + String( heap_stats.reserved_size)  );
}




/*
 unsigned long  heapSize()
{
   char   stackVariable;
   void   *heap;
   unsigned long result;
   heap  = malloc(4);
   result  = &stackVariable - heap;
   free(heap);
   return result;
}
 */

  void myShow(){
  
  char *heapend = sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi = mallinfo();
  Serial.println("Dynamic ram used: " + String( mi.uordblks));
 // Serial.println("Program static ram used " + String( &_end - ramstart));
  Serial.println("Stack ram used " + String( ramend - stack_ptr));
  Serial.println("My guess at free mem: " + String( stack_ptr - heapend + mi.fordblks));
  
  }
  


void myLedBlue_myThread(){
   while (true) {
    digitalWrite(LEDB, !digitalRead(LEDB));   //switch on / off
    mbed_stats_heap_get(&heap_info);
    mbed_stats_stack_get(&stack_info[0]);
    mbed_stats_stack_get_each(stack_info, MAX_THREAD_INFO);

    //debug("\nThis message is from debug function");
    Serial.println("MemoryStats:");

    Serial.println("Bytes allocated currently: "+String( heap_info.current_size));
    Serial.println("Max bytes allocated at a given time: "+String( heap_info.max_size));
    Serial.println("Cumulative sum of bytes ever allocated: "+String( heap_info.total_size));
    Serial.println("Current number of bytes allocated for the heap: "+String( heap_info.reserved_size));
    Serial.println("Current number of allocations: "+String( heap_info.alloc_cnt));
    Serial.println("Number of failed allocations: "+String( heap_info.alloc_fail_cnt));
    Serial.println();
    
    Serial.println("Cumulative Stack Info:");
    Serial.println("Maximum number of bytes used on the stack: "+String( stack_info[0].max_size));
    Serial.println("Current number of bytes allocated for the stack: "+String( stack_info[0].reserved_size));
    Serial.println("Number of stacks stats accumulated in the structure: "+String( stack_info[0].stack_cnt));
    Serial.println();
    
    Serial.println("Thread Stack Info:");
    for (int i = 0; i < MAX_THREAD_INFO; i++) {
        if (stack_info[i].thread_id != 0) {
            Serial.println("\tThread: " + String(i) );
            Serial.println("\t\tThread Id: 0x"+String(  stack_info[i].thread_id) + "8lX");
            Serial.println("\t\tMaximum number of bytes used on the stack: "+String( stack_info[i].max_size));
            Serial.println("\t\tCurrent number of bytes allocated for the stack: "+String( stack_info[i].reserved_size));
            Serial.println("\t\tNumber of stacks stats accumulated in the structure: "+String( stack_info[i].stack_cnt));
        }
    }





      Serial.println();
      Serial.println("The following is totally wrong");
      myShow();
      Serial.println();
      Serial.println("This next one looks better");
      print_memory_info();
     // Serial.println(heapSize());
      Serial.println();

      
      Serial.println(F("End of Data ------------------------"));
      Serial.println();
      Serial.println();


      
      ThisThread::sleep_for(myDelay);
      
   }
}

void setup() {
  Serial.begin(115200);
   pinMode(LEDB, OUTPUT);   // LEDB = blue, LEDG or LED_BUILTIN = green, LEDR = red 
   myThread.start(myLedBlue_myThread);
}

void loop() {
  // put your main code here, to run repeatedly:

}
