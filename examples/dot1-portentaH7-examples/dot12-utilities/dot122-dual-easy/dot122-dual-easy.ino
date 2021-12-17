// Easy Dual Core program setup
// If uploaded to the M7 Core erases what was on it and boots the M4 core
// If uploaded to the M4 Core erases what was on it.

//////////////////// Start All Core M7 Programing /////////////////////
#ifdef CORE_CM7 

void setup() { 
   bootM4();
}

void loop() {
 
}

#endif


//////////////////// End All Core M7 Programing /////////////////////

//////////////////// Start All Core M4 Programing /////////////////////

#ifdef CORE_CM4 

void setup() { 

}

void loop() {
 
}

#endif

//////////////////// End All Core M4 Programing /////////////////////
