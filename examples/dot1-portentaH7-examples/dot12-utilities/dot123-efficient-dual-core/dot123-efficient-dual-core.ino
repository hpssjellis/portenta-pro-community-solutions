// Efficient Dual Core program setup
// Good if lots of the code is duplicated
// If uploaded to the M7 Core erases what was on it and boots the M4 core
// If uploaded to the M4 Core erases what was on it.

//////////////////// Mixed M7 and M4 Core Programing /////////////////////

void setup() {
   
  #ifdef CORE_CM7  
     bootM4();
  #endif
  
  #ifdef CORE_CM4  
    // only M4 here
  #endif 
   
}

void loop() {
  
  #ifdef CORE_CM7  
    // only M7 Here
  #endif
  
  #ifdef CORE_CM4  
    // only M4 here
  #endif  
  
}
