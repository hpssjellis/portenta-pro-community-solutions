/*
*
* GNU GENERAL PUBLIC LICENSE
* Use at your own risk.
************************************************ Important stuff if needed ****************************************
*
* The following code will give you a local IP address to use. 
* Use the serial monitor to find your local IP. Then should work with or without serial monitor.
*
* Up to your router to assign outside Ethernet Access. This part can be complex.
* Make sure the etherenet cable is plugged into either the Breakout board or Vision Shield.
*
********************************************************** end ****************************************************
*
*/

#include <Arduino.h> // Only needed by https://platformio.org/
#include <Portenta_Ethernet.h>
#include <Ethernet.h>
#include "EthernetSSLClient.h"


EthernetSSLClient client;
/////////////////////////////// set below data ////////////////////////////////////////

// IPAddress server(52.54.163.195);  // IP to connect to - use either
char server[] = "io.adafruit.com";    // Server to connect 
uint16_t myPort = 443;  // insecure 80, secure 443 Not available yet on Portenta???

// note the slight difference between a webhook and what we need
// https://io.adafruit.com/api/v2/webhooks/feed/rvVztyR9G5D51ix6xoNkFCMMGG9T

String myAdafruitWebhook = "/api/v2/webhooks/feed/YOUR-ADAFRUIT-WEBHOOK-KEY";
String myData = "value=234";

/////////////////////////////// set the above ////////////////////////////////////////


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);  // what are these for??

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):

//EthernetServer server(80);
//EthernetClient client;





void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED Green pin mode  
  pinMode(LEDB, OUTPUT);   
            
  digitalWrite(LEDB, LOW);  
  digitalWrite(LED_BUILTIN, HIGH);  
  
  delay(5000);  // time to get serial monitor working if needed
  Serial.println("Wait a few seconds to plug in optional serial monitor");
  delay(5000);  
  Serial.println(".");
  delay(5000);   
  Serial.println(".");

  
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  //server.begin();
  Serial.print("This device is at ");
  Serial.println(Ethernet.localIP());
  digitalWrite(LEDB, HIGH);  
  digitalWrite(LED_BUILTIN, LOW); 

Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, myPort)) {

     Serial.print("Sending data, Connected to server: ");
     Serial.println(server);

     client.println("POST " + myAdafruitWebhook + " HTTP/1.1");
     client.println("connection: close");
     client.println("content-type: application/x-www-form-urlencoded;charset=utf-8");   //application/x-www-form-urlencoded   application/json
     client.println("host: io.adafruit.com");
     client.println("user-agent: arduino");
     client.println("content-length: " + String(myData.length()) );
     client.println();
     client.println(myData);   
     client.println();
     client.println();

  }
  
}


void loop() {
  // listen for incoming clients
 // EthernetClient client = server.available();

  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


