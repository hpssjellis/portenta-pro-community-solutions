/*
 *    Arduino PortentaH7 SSLwebsocket example 
 *    Note: This is not professionally made and is only for Maker activity
 * 
 * 
 *    Research:
 *    https://github.com/janjongboom/mbed-ws/blob/master/source/ws_client_base.h
 *    https://www.amebaiot.com/zh/rtl8195-arduino-api-wifisslclient/
 *    https://os.mbed.com/teams/mbed_example/code/WebSocketClient//file/efa2c147bee1/Websocket.cpp/
 *    example at   https://github.com/ambiot/amb1_arduino/blob/dev/Arduino_package/hardware/libraries/WiFi/examples/WiFiSSLClient/WiFiSSLClient.ino
 *
*/

#include <WiFi.h>
#include <WiFiSSLClient.h>




char ssid[] = "";        // your network SSID (name)
char pass[] = "";                  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                          // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Link to your own websocket server in this format
char HTTPS_SERVER[] = "4fh42v-8080.preview.csb.app";
//char HTTPS_SERVER[] = "8080-hpssjellis-gitpodporten-685bxri4ie6.ws-us80.gitpod.io";
char HTTPS_PATH[] = "/";

// Note: Gitpod server link looks like https://8080-hpssjellis-gitpodporten-685bxri4ie6.ws-us80.gitpod.io
//    Note: If you open this URL you get to the browser websocket client ran by the websoket server on codesandbox
//    https://4fh42v-8080.preview.csb.app/

// no certificate needed, Arduino has made it already a part of the SSLclient.

WiFiSSLClient client;
unsigned long mySendMillis, myWaitToReadMillis;
int mySendDuration = 10000;   // send test data every x milliseconds
int myWaitToReadDuration = 20;   // delay time before reading data so no conflicts after a write
bool myCanReadNow = true;
bool myOnlyOnce = false;







void setup() {
    //Initialize serial and wait for port to open
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);

    while (!Serial && millis() < 5000); // either serial monitor or wait 5 seconds
    digitalWrite(LEDB, HIGH);   



    // attempt to connect to Wifi network
    while (status != WL_CONNECTED) {
        digitalWrite(LEDG, !digitalRead(LEDG));  // flip onboard Green LED on and off
        Serial.print("\r\n Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        status = WiFi.begin(ssid,pass);

        // wait for connection
        delay(3000);
    }
    digitalWrite(LEDG, HIGH);   
    digitalWrite(LEDB, LOW);  //flash of blue once connected
    delay(1000); 
    digitalWrite(LEDB, HIGH);   
    Serial.println("Connected to wifi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // client.setRootCA((unsigned char*)rootCABuff);  // already done by arduino

    if (client.connect(HTTPS_SERVER, 443)) {
        Serial.println("connected to server");
        // Make a HTTP request:
        client.print("GET ");
        client.print(HTTPS_PATH);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(HTTPS_SERVER);
        client.println("Upgrade: websocket");
        client.println("Connection: Upgrade");
        
        //  use a Base64 encoder/decoder to make your own Sec-WebSocket-Key
        //  https://www.rapidtables.com/web/tools/base64-decode.html
        client.println("Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==");
        client.println("Sec-WebSocket-Version: 13");
        client.println();
    } else {
        Serial.println("connected to server failed");
    }
    delay(100);

    mySendMillis = millis();
    myWaitToReadMillis = millis();
}

void loop() {
   
    if (myOnlyOnce && (millis() - myWaitToReadMillis) >= myWaitToReadDuration ) {
        myCanReadNow = true;  // after a delay when writing allow reading again 
        myOnlyOnce = false;   // so not flushing the client continuously
        client.flush();       // atttempt to solve random crash
        
        Serial.println();
        digitalWrite(LEDB, HIGH);   
    }
    
    // if there are incoming bytes available
    // from the server, read them and print them
    while (client.available() && myCanReadNow) {  // no LED on read as it would be constantly going
        char c = client.read();
       // Serial.print(c, HEX);  // good to debug
      //  Serial.print(",");
        Serial.print(c);   // show the characters // somehow causing issues after sending data 
    

       if ((millis() - mySendMillis) >= mySendDuration ) {
        
          digitalWrite(LEDB, LOW);   // flash of blue when sending data
          mySendMillis = millis();    
          myWaitToReadMillis = millis(); 
          myCanReadNow = false;  // stop the ability to read while writing
          myOnlyOnce = true;
          Serial.println(); 
          Serial.println("Send: Hello"); 
          // Testing opcodes, mask, data
          // const char msg[] = {0x81, 0x85,    0x01, 0x02, 0x03, 0x04,   0x49, 0x67, 0x6F, 0x68, 0x6E};  // Sends HELLO as TEXT
          // client.write((const uint8_t*)msg, strlen(msg)); 

          const char msg[] = "Hello6789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";  // more standard way to send it
        
          // mySendTXT(1, "Hello", 5);   // this also works  

          
          // const char msg[] = "Hello";  // more standard way to send it
          mySendTXT(1, msg, strlen(msg)); //1=text or 2=binary or anything else = binary
           
                      
       } // now send
    }  // while connected



    // if the server's disconnected, stop the client
    if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();

        digitalWrite(LEDR, LOW);   
        digitalWrite(LEDG, HIGH);   
        digitalWrite(LEDB, LOW);   
        // do nothing
        while (true);
    }
}


//void mySendTXT(const char *myMessage, int mySize){
//void mySendTXT(const char *myMessage, uint32_t mySize){
void mySendTXT(int my1TextElseBinary, const char *myMessage, uint32_t mySize){

// Only for Text so far
  
  // char *myPayload;
  // char myPayload[2+4+mySize];
   uint8_t myHeader[10];
   uint32_t  myHeaderLength = 2;  //default2   10 is max based on size of data to send
   uint8_t myMask[4];
   
   // set binary or text 
   myHeader[0]={0x82};   // FIN 80 and TEXT = 1 BINARY = 2
   if (my1TextElseBinary == 1){
      myHeader[0]={0x81};   // FIN 80 and TEXT = 1 BINARY = 2
   }
   // determine header+1 length 

        if (mySize < 126) {
            myHeader[1] = mySize| (1 << 7);
            myHeaderLength = 1+1;
        }
        else if (mySize < 65535) {
            myHeader[1] = 126 | (1 << 7);
            myHeader[2] = (mySize >> 8) & 0xff;
            myHeader[3] = mySize & 0xff;
            myHeaderLength = 3+1;
            //myHeaderLength = 3;
        }
        else {
            myHeader[1] = 127 | (1 << 7);
            for (int i = 0; i < 8; i++) {
                myHeader[i+1] = (mySize >> i*8) & 0xff;
            }
            myHeaderLength = 9+1;
           // myHeaderLength = 9;
        }
        
   uint8_t myPayload[myHeaderLength+4+mySize];  // define a big enough character array possibly should be a char pointer
 
   // Show input
   Serial.print("myMessage: ");
   Serial.println(myMessage);
   Serial.print("mySize: ");
   Serial.println(mySize);
   
   // Set headers and mask
   for (int k = 0; k < 4; k++) {
      myMask[k]=random(1,99);  // this is good
     // myMask[k]={0x01};  // just testing
   }


   for (int m = 0; m < myHeaderLength; m++) {   
      myPayload[m] = myHeader[m];
   }

   // Now the 4 mask bytes
   myPayload[myHeaderLength+0] = myMask[0];
   myPayload[myHeaderLength+1] = myMask[1];
   myPayload[myHeaderLength+2] = myMask[2];
   myPayload[myHeaderLength+3] = myMask[3];

   for (int j = 0; j < mySize; j++) {
      myPayload[j+myHeaderLength+4] = myMessage[j] ^ myMask[j % 4];
    }

  // Show output and send it
   Serial.print("myPayload: ");
   for (int i = 0; i < 2+4+mySize; i++) { 
      Serial.print((char)myPayload[i], HEX);
      Serial.print(",");
   }
   Serial.println();
   Serial.print("myHeaderLength: ");
   Serial.println(myHeaderLength);
   //Serial.print("myPayload: ");
   //Serial.println(myPayload);
   Serial.print("sizeof(myPayload): ");
   Serial.println(sizeof(myPayload));

   //now send new payload to websocket
   client.write((const uint8_t*)myPayload, myHeaderLength+4+mySize); 
}



void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

