/*
  Portenta - TestSDCARD

  The sketch shows how to mount an SDCARD and list its content.
  then add a file.

  Note: SD Card seems finisky, takes a few times to load the filesystem

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
FILE *myFile;
char buffer[40];   // must be long enough for all data points with commas: 56,34,23,56


void setup() {
  Serial.begin(9600);
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
}

void loop() {

  int myTime = millis()/1000; // seconds since the sketch began
  //char myFileName[] = "fs/folder1/00000000.json";  // works if folder1 pre-made
  char myFileName[] = "fs/00000000.json";   // fs/ needs to be there think fileSystem
  int myExtensionLength = 4;  // .txt = 3 .json = 4
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
    Serial.println(myFileName);

  //  fprintf(myFile,"test \r\n");
    fprintf(myFile,"{\"protected\":{\"ver\":\"v1\",\"alg\":\"none\",\"iat\":1603914126},");
    fprintf(myFile,"\"signature\":\"0\",\"payload\":{\"device_type\":\"TEST\",\"interval_ms\":10,");
    fprintf(myFile,"\"sensors\":[{\"name\":\"W\",\"units\":\"N/A\"},{\"name\":\"X\",\"units\":\"N/A\"},{\"name\":\"Y\",\"units\":\"N/A\"},{\"name\":\"Z\",\"units\":\"N/A\"}],");
    fprintf(myFile,"\"values\":[[");
    
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
    strcat(buffer, buffer1);
    strcat(buffer, myComma);
    strcat(buffer, buffer2);
    strcat(buffer, myComma);
    strcat(buffer, buffer3);
    strcat(buffer, myComma);
    strcat(buffer, buffer4);
   
    fprintf(myFile, buffer);
    fprintf(myFile,"]]}}\r\n");

  fclose(myFile);
  delay(30000);


}
