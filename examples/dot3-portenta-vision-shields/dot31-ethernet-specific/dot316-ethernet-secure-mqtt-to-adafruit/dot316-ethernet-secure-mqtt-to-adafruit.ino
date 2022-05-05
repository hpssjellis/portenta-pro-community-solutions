
/*
 * Uses the library MQTT.h from https://github.com/256dpi/arduino-mqtt 
 * GNU GENERAL PUBLIC LICENSE
 * but made for the PortentaH7
 * By Rocksetta
 * Use at your own risk!
*/

#include <Arduino.h> // Only needed by https://platformio.org/
#include <Portenta_Ethernet.h>
#include <Ethernet.h>
#include <MQTT.h>
#include "EthernetSSLClient.h"




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



////////////////////////////////// You need to enter these ///////////////////////////////////////////////


#define THE_SERVER "io.adafruit.com"
int myPort = 8883;   //ssl 8883,  insecure 1883


#define THE_RANDOM_ID "random345445"   // if using several devices each one needs this to be unique
#define THE_USER_NAME "yourUsername"
#define THE_ADAFRUIT_ACTIVE_KEY "aio_aaaaaaaaaaaayour-webhook-keyaaaaaaaaaaaaa"
#define THE_TOPIC_USER_FEEDS_KEY "yourUSER/feeds/yourFeedName"   // such as         joeUser/feeds/led1



long myMillisSecondDelay = 30000;   // 30000 = 30 seconds



////////////////////////////// End the entry area //////////////////////////////////////////////////////

//int status = WL_IDLE_STATUS;

//
//EthernetSSLClient client;
//EthernetClient netSSL;
EthernetSSLClient netSSL;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {

  /*
  Serial.print("Checking wifi to: " + String(ssid));
  while (status != WL_CONNECTED) {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(3000);
  }
  */

  Serial.print("\nWifi Connected, now connecting to the MQTT Broker ...");
  
                         // clientID (a random unique number), username, activeKey, bool for skip
  while (!client.connect(THE_RANDOM_ID, THE_USER_NAME, THE_ADAFRUIT_ACTIVE_KEY)) {
    Serial.print(".");
    delay(3000);
  }

  Serial.println("\nconnected to : " + String(THE_SERVER) );

  client.subscribe(THE_TOPIC_USER_FEEDS_KEY);
  // client.unsubscribe(THE_TOPIC_USER_FEEDS_KEY);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " : " + payload);

  // Note: Do not use the variable "client" here as it will lock up
}






void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED Green pin mode  
  pinMode(LEDB, OUTPUT);             
  digitalWrite(LEDB, HIGH);  
  digitalWrite(LED_BUILTIN, LOW);  
  
  delay(5000);  // time to get serial monitor working if needed
  Serial.println("Wait a few seconds to plug in serial");
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


  // MQTT brokers usually use port 8883 for secure connections.
  client.begin(THE_SERVER, myPort, netSSL);
  client.onMessage(messageReceived);

  connect();


  // start the server
 // server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  digitalWrite(LEDB, LOW);  
  digitalWrite(LED_BUILTIN, HIGH); 
}


void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every 30 seconds.
  if (millis() - lastMillis > myMillisSecondDelay) {
    lastMillis = millis();
    
    // Send whatever reading is at A0 from 0->1023 even if nothing is connected to it
    client.publish(THE_TOPIC_USER_FEEDS_KEY, String(analogRead(A0)) );
  }
}  
