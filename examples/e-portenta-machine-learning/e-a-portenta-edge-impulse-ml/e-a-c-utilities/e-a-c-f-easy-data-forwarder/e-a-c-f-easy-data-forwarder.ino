
// for the Arduino Portenta

//#define FREQUENCY_HZ        6
#define INTERVAL_MS        400     //(1000 / (FREQUENCY_HZ + 1))

static unsigned long last_interval_ms = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Started");
    pinMode(LEDB, OUTPUT);
}

void loop() {

    if (millis() > last_interval_ms + INTERVAL_MS) {
   
        digitalWrite(LEDB, !digitalRead(LEDB)); // flip internal LED
        last_interval_ms = millis();

        int   w = analogRead(A1);
        int   x = analogRead(A2);
        float y = analogRead(A3)/10.0;
        float z = analogRead(A4)/10.0;

        Serial.println(String(w) + ',' + String(x) + ',' + String(y,3) + ',' + String(z,3) );

    }
}
