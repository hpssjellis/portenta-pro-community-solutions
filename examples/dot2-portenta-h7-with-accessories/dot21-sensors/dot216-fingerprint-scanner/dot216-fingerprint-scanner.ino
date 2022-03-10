/* Hardware setup - FPS connected to Photon using JST SH Jumper 4 Wire Assembly (https://www.sparkfun.com/products/10359)
	If the JST SH jumper wires were numbered left to right as 1-4 (with black wire being Wire 2), the connections would be:	
	FPS Wire 1 (Red) = VCC --> Portenta 3.3V
	FPS Wire 2 (Black) = GND --> Portenta GND
	FPS Wire 3 (Yellow) = RX --> Portenta TX   D14
	FPS Wire 4 (Green) = TX --> Portenta RX    D13
	
	Built-in green LED on FPS will light up if wiring is correct
    
    Note in the extra folders is the enroll.ino code that enters fingerprints.
    Note: The FPS needs to warm up for about 10 minutes before it is stable.
    Somedays works really well, other days a bit flaky
*/






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
