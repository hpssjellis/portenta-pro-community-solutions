/*
  Portenta - TestSDCARD

Make a CSV file
----------------------------------------
Connect D6 -->push button --> 3V3 to use it as a sensor activation button
Connect A1 through A4 to voltage dividers to take 2 prong sensor readings
example GND --> 1000 ohm resistor --> A1 AND also A1 --> light sensor --> 3V3

Note: Makes MULTIPLE CSV files for edgeimpulse.com upload
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
int myDelay = 10000;  // delay in milliseconds between sensor read
static unsigned long myLastMillis = 0;


void setup() {
  Serial.begin(9600);
  pinMode(D6, INPUT_PULLDOWN); // to altenatively activate a sensor read
  pinMode(LEDB, OUTPUT);
  
  while (!Serial);   // might want to get rid of this after it works

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
 Serial.println("------------------------- Done SD Card Check --------------------------------"); 


}

void loop() {


 int myTime = millis()/1000; // seconds since the sketch began
 
 if ((millis() > myLastMillis + myDelay) || digitalRead(D6) ) {
   myLastMillis = millis();  // note if button push resets timer
   digitalWrite(LEDB, !digitalRead(LEDB)); // flip internal LED
  //char myFileName[] = "fs/folder1/00000000.json";  // works if folder1 pre-made
  char myFileName[] = "fs/00000000.csv";   // fs/ needs to be there think fileSystem
  int myExtensionLength = 3;  // .txt = 3 .json = 4
  // does anyone understand why the  + '0' is needed below???
  myFileName[sizeof(myFileName)- myExtensionLength - 10] = myTime/10000000 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 9] = myTime/1000000 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 8] = myTime/100000 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 7] = myTime/10000 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 6] = myTime/1000 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 5] = myTime/100 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 4] = myTime/10 % 10 + '0';
  myFileName[sizeof(myFileName)- myExtensionLength - 3] = myTime % 10 + '0';
  // Can make a new file but can't make a new folder
         
   myFile = fopen(myFileName, "a");  // "a" for append or make it if file not there
   
    // add the CSV file titles with no spaces
    fprintf(myFile,"timestamp,W,X,Y,Z\r\n");


    
    // now lets get the time values in seconds 
    int myMillis = millis(); // milliseconds since the sketch began  
    char bufferTime[12]; 
    itoa( myMillis, bufferTime, 10);  
    
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

// fake second row of data


    
    // now lets get the time values in seconds   
   //char bufferTime[12]; 
   myMillis += 1;
    itoa( myMillis, bufferTime, 10);  
    
    // now lets get the sensor values     
    //char buffer1[7]; 
    itoa( analogRead(A1), buffer1, 10);
    
    //char buffer2[7]; 
    itoa( analogRead(A2), buffer2, 10);
    
    //char buffer3[7]; 
    itoa( analogRead(A3), buffer3, 10);
    
    //char buffer4[7]; 
    itoa( analogRead(A4), buffer4, 10);

    
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
    Serial.println("File saved as: "+ String(myFileName));
    
    if (digitalRead(D6)) {
       delay(1000); // gives time between button pushes
    }
  
 }  // end myDelay or D6 buttonPush
  
}
