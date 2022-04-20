 /*By rocksetta
 * Use at your own risk
 * 
*/

#include <Arduino.h> // Only needed by https://platformio.org/
#include "WiFi.h"
#include "WiFiSSLClient.h"


///////please enter your sensitive data in the Secret tab/arduino_secrets.h

// Choose either the following arduino_secrets.h tab and bracket out the next 2 lines after it
// That route is more secure.
// Or just change the lines below for your Network and Password. Eaier but not as secure
// Since if you share this file it will have your info on it


////////////////////////////// Set the below values /////////////////////////////////////////////


//#include "arduino_secrets.h"   // more safe
#define SECRET_SSID "yourSSID"
#define SECRET_PASS "yourPASS"


// IPAddress server(52.54.163.195);  // IP to connect to - use either
char server[] = "io.adafruit.com";    // Server to connect 

// note the slight difference between a webhook and what we need
// https://io.adafruit.com/api/v2/webhooks/feed/rvVztyR9G5D51ix6xoNkFCMMGG9T

String myAdafruitWebhook = "/api/v2/webhooks/feed/rvVztyR9G5D51ix6xoNkFCMMGG9T";    // needs your webhook
String myData = "value=234";


/////////////////////////////// set the above ////////////////////////////////////////


char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiSSLClient client;

void setup() {

  Serial.begin(115200);
  //while (!Serial) {} // wait for serial port to connect. I DON'T LIKE WAITING
  
  // Below non-blocking and gives time for serial monitor
  digitalWrite(LEDB, LOW);  
  delay(5000);
  Serial.println("Wait a bit to connect serial monitor");
  digitalWrite(LEDB, HIGH);  
  delay(5000);
  Serial.println("Wait a bit");
  digitalWrite(LEDB, LOW);  
  delay(5000);
  Serial.println("Wait a bit");
  
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(LEDB, OUTPUT);     
  digitalWrite(LEDB, LOW);          // blue on while connecting
  digitalWrite(LED_BUILTIN, HIGH);          

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  digitalWrite(LEDB, HIGH);          
  digitalWrite(LED_BUILTIN, LOW);    // Green on When Connected
  
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

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {

     Serial.println("Conected to Server! Sending data");

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
        
         
