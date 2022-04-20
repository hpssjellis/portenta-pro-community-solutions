/*
 * Uses the library MQTT.h from https://github.com/256dpi/arduino-mqtt 
 * but made for the PortentaH7
 * By Rocksetta
 * Use at your own risk!
*/

#include <Arduino.h> // Only needed by https://platformio.org/
#include "WiFi.h"
#include "WiFiSSLClient.h"

#include <MQTT.h>

////////////////////////////////// You need to enter these ///////////////////////////////////////////////

//#include "arduino_secrets.h"   // more safe to move the following
#define SECRET_SSID "yourSSID"
#define SECRET_PASS "yourPASS"

#define THE_SERVER "io.adafruit.com"
int myPort = 8883;   //ssl 8883,  insecure 1883


#define THE_RANDOM_ID "random2345345"   // if using several devices each one needs this to be unique
#define THE_USER_NAME "yourUSER"
#define THE_ADAFRUIT_ACTIVE_KEY "aio_aaaaa-your-active-key-aaaaaa"
#define THE_TOPIC_USER_FEEDS_KEY "yourUSER/feeds/yourFeedName"   // such as         joeUser/feeds/led1

long myMillisSecondDelay = 30000;   // 30000 = 30 seconds



////////////////////////////// End the entry area //////////////////////////////////////////////////////

int status = WL_IDLE_STATUS;


char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

WiFiSSLClient netSSL;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("Checking wifi to: " + String(ssid));
  while (status != WL_CONNECTED) {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(3000);
  }

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
  
  Serial.begin(115200);
  //while (!Serial) {} // wait for serial port to connect. I DON'T LIKE WAITING
 
   pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(LEDB, OUTPUT);   
  digitalWrite(LEDB, LOW);  
  delay(5000);
  Serial.println("Wait a bit to connect serial monitor");
  digitalWrite(LEDB, HIGH);  
  delay(5000);
  Serial.println("Wait a bit");
  digitalWrite(LEDB, LOW);  
  delay(5000);
  Serial.println("Wait a bit");
  
  
  digitalWrite(LEDB, LOW);          // blue on while connecting
  digitalWrite(LED_BUILTIN, HIGH);          

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
  // MQTT brokers usually use port 8883 for secure connections.
  client.begin(THE_SERVER, myPort, netSSL);
  client.onMessage(messageReceived);

  connect();
  digitalWrite(LEDB, HIGH);          // Green on while connected
  digitalWrite(LED_BUILTIN, LOW); 
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
  
  
