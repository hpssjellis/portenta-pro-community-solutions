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


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  delay(5000); // time to connect serial
  Serial.println("Connect serial if needed");
  delay(5000);
  //while (!Serial);  // blocking call

  Serial.println("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
     Serial.println("No SD Card filesystem found, please check SD Card on computer and manually format if needed.");
     // int err = fs.reformat(&block_device);  // seriously don't want to format your good data
  }

}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // flips LED on and off
  
  // Make a folder if needed
  mkdir("fs/myFolder2",0777);                     // 0777 full access permissions linux style 
   
  char myFileName[] = "fs/myFolder2/test2.txt";   // "fs/" needs to be there, think fileSystem
 
  FILE *myFile = fopen(myFileName, "w");          // "a" for append (add to file), "w" write, "r" read ??
 
     Serial.println(myFileName);
     fprintf(myFile,"Test how cool this is \r\n");
     fprintf(myFile,"Also this line \r\n");
   
  fclose(myFile);

  Serial.println("------------------------- Done Writing file --------------------------------");
  delay(10);
   
  unsigned char c; 
  FILE *fp = fopen(myFileName, "r");              // "r" read only
     while (!feof(fp)){                           // while not end of file
        c=fgetc(fp);                              // get a character/byte from the file
        //printf("Read from file %02x\n\r",c);    // and show it in hex format
        Serial.print((char)c);                    // show it as a text character
     }
  fclose(fp); 
  Serial.println("------------------------- Done Showing file --------------------------------");

  delay(10000);   // wait a bit
  
}
