/*
  Portenta - TestSDCARD

Make a CSV file
----------------------------------------
Connect D6 -->push button --> 3V3 to use it as a sensor activation button
Connect A1 through A4 to voltage dividers to take 2 prong sensor readings
example GND --> 1000 ohm resistor --> A1 AND also A1 --> light sensor --> 3V3


------------------------------------------
  The sketch shows how to mount an SDCARD and list its content.
  then add a file.

  Note: SD Card seems finicky, takes a few times to load the filesystem

  The circuit:
   - Portenta H7 + Vision Shield
   or
   - Portenta H7 + Portenta Breakout

  This example code is in the public domain.
*/
#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");

// Global Variables
FILE *myFile;
char buffer[50];   // must be long enough for all data points with commas: 56,34,23,56
char myFileName[] = "fs/myData01.csv";   // fs/ needs to be there, think fileSystem, can add a pre-made folder
int myDelay = 10000;  // delay in milliseconds between sensor read
static unsigned long myLastMillis = 0;


void setup() {
  Serial.begin(9600);
  pinMode(D6, INPUT_PULLDOWN); // to altenatively activate a sensor read
  pinMode(LEDB, OUTPUT);
  while (!Serial);

  Serial.println("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    Serial.println("No filesystem found, please check on computer and manually format if needed.");
  //  err = fs.reformat(&block_device);  // seriously don't want to format your good data
  }
  if (err) {
     Serial.println("Error formatting SDCARD ");
     while(1);
  }
  
  DIR *dir;
  struct dirent *ent;
  int dirIndex = 0;

  Serial.println("List SDCARD content: ");
  if ((dir = opendir("/fs")) != NULL) {
    // Print all the files and directories within directory (not recursively)
    while ((ent = readdir (dir)) != NULL) {
      Serial.println(ent->d_name);
      dirIndex++;
    }
    closedir (dir);
  } else {
    // Could not open directory
    Serial.println("Error opening SDCARD\n");
    while(1);
  }
  if(dirIndex == 0) {
    Serial.println("Empty SDCARD");
  }
 Serial.println("------------------------- Done --------------------------------"); 


 myFile = fopen(myFileName, "a");  // "a" for append or make it if file not there
    Serial.println(myFileName);
    
    // add the CSV file titles
    fprintf(myFile,"timestamp,W,X,Y,Z \r\n");
   
 fclose(myFile); // safer to close the file often

}

void loop() {

 int myMillis = millis(); // milliseconds since the sketch began
 int myTime = millis()/1000; // seconds since the sketch began
 if ((millis() > myLastMillis + myDelay) || digitalRead(D6) ) {
   myLastMillis = millis();  // note if button push resets timer
   digitalWrite(LEDB, !digitalRead(LEDB)); // flip internal LED
           
   myFile = fopen(myFileName, "a");  // "a" for append or make it if file not there
   
    // now lets get the time values in seconds   
    char bufferTime[12]; 
    itoa( myTime, bufferTime, 10);
    
    // now lets get the sensor values     
    char buffer1[7]; 
    itoa( analogRead(A1), buffer1, 10);
    
    char buffer2[7]; 
    itoa( analogRead(A2), buffer2, 10);
    
    char buffer3[7]; 
    itoa( analogRead(A3), buffer3, 10);
    
    char buffer4[7]; 
    itoa( analogRead(A4), buffer4, 10);

    char myComma[] = ",";
    memcpy (buffer, "", sizeof(buffer) );  // one way to delete the old buffer
    strcat(buffer, bufferTime);
    strcat(buffer, myComma);
    strcat(buffer, buffer1);
    strcat(buffer, myComma);
    strcat(buffer, buffer2);
    strcat(buffer, myComma);
    strcat(buffer, buffer3);
    strcat(buffer, myComma);
    strcat(buffer, buffer4);
   
    fprintf(myFile, buffer);
    fprintf(myFile,"\r\n");  // next line

  fclose(myFile);  // important to close file before crap happens
  if (digitalRead(D6)) {
    delay(500); // gives time between button pushes
  }
 }  // end myDelay or D6 buttonPush
  
}
