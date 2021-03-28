/*
 * 
 * On Nano33Ble UART Pins
 * GND 4th from bottom right
 * UART receive Pin RX 2nd from bottom right
 * UART Transmit pin TX 1st from bottom right
 * 
 * on PortentaH7
 * GND is GND
 * UART receive Pin RX     D13
 * UART Transmit pin TX    D14
 * 
 * 
 * 
 * 
 */



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(115200);  // UART: TX and RX
  
}

void loop() {
   digitalWrite(LED_BUILTIN, LOW);  // flash on-board LED no matter grounding
   delay(400);
   digitalWrite(LED_BUILTIN, HIGH);
   delay(400);
   digitalWrite(LED_BUILTIN, LOW);
   delay(400);
   
   Serial1.println("Hello from an external Arduino using UART: RX and TX");
   delay(2000);
  
} 






/*
 * Code on the Portenta looks something like
 * * Note these Portenta defines
 * 
 * #define PIN_SERIAL_RX (13ul)
 * #define PIN_SERIAL_TX (14ul)
 * 
 * #define SERIAL2_TX      PA_15
 * #define SERIAL2_RX      PF_6
 * #define SERIAL2_RTS     PF_8
 * #define SERIAL2_CTS     PF_9
 * 
 * 
 * #define SERIAL1_TX      (digitalPinToPinName(PIN_SERIAL_TX))
 * #define SERIAL1_RX      (digitalPinToPinName(PIN_SERIAL_RX))

void setup() {
  pinMode(LEDB, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  
}

void loop() {

  Serial.println("waiting...");
  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
  
}




 
 */

