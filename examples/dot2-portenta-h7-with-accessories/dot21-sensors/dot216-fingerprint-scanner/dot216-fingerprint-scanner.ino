#include <Arduino.h> // Only needed by https://platformio.org/
#include <GT5X.h>

/* Count templates */


GT5X finger(&Serial1);
GT5X_DeviceInfo info;

void setup() {
    Serial.begin(9600);
    Serial.println("COUNT TEMPLATES test");

     Serial1.begin(9600);
     finger.set_led(true);
     delay(5000);
     finger.set_led(false);
     delay(5000);
     finger.set_led(true);
     delay(5000);
     
    if (finger.begin(&info)) {
        Serial.println("Found fingerprint sensor!");
        Serial.print("Firmware Version: "); Serial.println(info.fwversion, HEX);
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) yield();
    }

    while (Serial.read() != -1);
    Serial.println("Send any character to start.\n");
    while (Serial.available() == 0) yield();
}

void loop() {
    uint16_t count;
    uint16_t rc = finger.get_enrolled_count(&count);
    if (rc != GT5X_OK) {
        Serial.println("Could not get count!");
        return;
    }
    
    Serial.print(count); Serial.println(" templates in database.\r\n");
    while (1) yield();
}
