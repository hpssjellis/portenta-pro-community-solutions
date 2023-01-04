/*
 *  WiFi Web Server 
 *  Uses an SD card and servers both an html file and PNG image
 *  
 *  Enter you wifi and password in the code below 
 *
 *
 * If the IP address of your board is yourAddress:
 * http://yourAddress/H turns the LED on
 * http://yourAddress/L turns it off
 *
 *
 * created 25 Nov 2012
 * by Tom Igoe
 * adapted by @rocksetta  aka Jeremy Ellis Jan 2023
 * for the PortentaH7 with vision shield and a formatted sd card.
 * 
 * File names are written near where they are used for simplicity.
 * 
 */


#include <Arduino.h> // Only needed by https://platformio.org/
#include "WiFi.h"

// for SD card
#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");


int mode = 0; 




// Choose either the following arduino_secrets.h tab and bracket out the next 2 lines after it
// That route is more secure.
// Or just change the lines below for your Network and Password. Eaier but not as secure
// Since if you share this file it will have your info on it

//#include "arduino_secrets.h"   // more safe
#define SECRET_SSID ""
#define SECRET_PASS ""
  


char ssid[] = SECRET_SSID;    // Changing the secret strings to a character array
char pass[] = SECRET_PASS;    
int keyIndex = 0;              

int status = WL_IDLE_STATUS;
WiFiServer server(80);


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}



void setup() {
  Serial.begin(115200);      // initialize serial communication
  delay(5000);
  Serial.println("Wait a bit to connect serial monitor");
  delay(5000);
  Serial.println("Wait a bit");
  delay(5000);
  Serial.println("Wait a bit");
  
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  pinMode(LEDB, OUTPUT);      // set the LED pin mode
  digitalWrite(LEDB, LOW);  



  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
  digitalWrite(LEDB, HIGH);  
  digitalWrite(LED_BUILTIN, LOW); 


  Serial.println("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
     Serial.println("No SD Card filesystem found, please check SD Card on computer and manually format if needed.");
     // int err = fs.reformat(&block_device);  // seriously don't want to format your good data
  }




  mkdir("fs/myFolder1",0555);  
  char myFileName[] = "fs/myFolder1/index.html";   // "fs/" needs to be there, think fileSystem
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


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    int x1 = analogRead(A1);                // read A1
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");

            
            if (mode == 2 ){      // show a PNG on it's own page     
              client.println("Content-Type: image/png");
              client.println();
              mkdir("fs/myFolder1",0555);  
              char myFileName[] = "fs/myFolder1/img01.png";   // "fs/" needs to be there, think fileSystem
              char buffer[128];
              FILE *fp = fopen(myFileName, "rb");              // "r" read only
              //while(fgets(buffer, 128, fp)) {           // "r" read only
              while(fread(buffer, 1, 128, fp)) {
                 client.write((uint8_t*)buffer, 128); 
                // wait(1);
                delayMicroseconds(200);
              }
              fclose(fp); 
              Serial.println("------------------------- Done Showing file --------------------------------");
              delay(1);
              client.stop();
              currentLine="";
              break;
              } 




            // generic header for all these webpages following
            client.println("Content-type:text/html");
           //client.println("{answer:42}");
            client.println();





            if (mode == 4 ){  // show html file inside this webpage
                mkdir("fs/myFolder1", 0555);  
                char myFileName[] = "fs/myFolder1/index.html";   // "fs/" needs to be there, think fileSystem
                char line[256];
                FILE *fp = fopen(myFileName, "r");              // "r" read only
                while (fgets(line, sizeof(line), fp)){           // while not end of file
                    // remove the last character (likely newline or carriage return)
                    int len = strlen(line);
                    if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
                        line[len-1] = '\0';
                    }
                    //printf("Read from file: %s", line);       // show the line as a string
                    Serial.print(line);                         // show it as a text character
                    client.print(line);                         // print to web page
                }
                fclose(fp); 
                Serial.println("------------------------- Done Showing file --------------------------------");
                delay(1);
                client.stop();
                currentLine="";
                break;
            }







            

            // the content of the HTTP response follows the header:
            client.print("<input type=button value='LED_BUILTIN Off' onclick='{location=\"/H\"}'>");
            client.print("<input type=button value='LED_BUILTIN On' onclick='{location=\"/L\"}'>");
            client.print("<input type=button value='Print html file from SD CARD to this page' onclick='{location=\"/myAddHtml\"}'>");
            client.print("<input type=button value='Print html file from SD CARD to a fresh page' onclick='{location=\"/SD-cardHtml\"}'>");
            client.print("<input type=button value='Load Image img01.png from SD card to own page' onclick='{location=\"/imageOwn\"}'>");
            client.print("<input type=button value='Load Image img01.png from sd card here' onclick='{location=\"/imageHere\"}'>");
            client.print("<br> AnalogRead(A1); = "+ String(x1) );  
            if (mode==3){
              client.print("<br><img width=320 height=320 src='http://255.255.255.255/imageOwn' />");  // needs your local IP. See serial monitor
            }
            if (mode == 1 ){  // show html file inside this webpage
                mkdir("fs/myFolder1", 0555);  
                char myFileName[] = "fs/myFolder1/index.html";   // "fs/" needs to be there, think fileSystem
                char line[256];
                FILE *fp = fopen(myFileName, "r");              // "r" read only
                while (fgets(line, sizeof(line), fp)){           // while not end of file
                    //printf("Read from file: %s", line);       // show the line as a string
                    Serial.print(line);                         // show it as a line of text characters
                    client.print(line);                         // print to web page
                }
                fclose(fp); 
                Serial.println("------------------------- Done Showing file --------------------------------");
            }



            // The HTTP response ends with another blank line:
            client.println();    
            // break out of the while loop:
            break;
           }else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
          mode=-2;
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
          mode=-1;
        }        
        if (currentLine.endsWith("GET /myAddHtml")) {           // show SD card HTML file in this page
          mode=1;               
        }       
        if (currentLine.endsWith("GET /imageOwn")) {           // show SD card PNG on it's own page
          mode=2;               
        }         
        if (currentLine.endsWith("GET /imageHere")) {           // show SD card PNG embedded in this page
          mode=3;               
        }          
        if (currentLine.endsWith("GET /SD-cardHtml")) {           // show SD card html file on it's own page
          mode=4;               
        }        
      }
    }

    
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);  
    Serial.println("");
  }
  
}
