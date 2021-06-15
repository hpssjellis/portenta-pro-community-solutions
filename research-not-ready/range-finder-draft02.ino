// pulseIn Example

const byte INPIN = 5;

void setup() {
   Serial.begin(9600);
   pinMode(INPIN, INPUT);
   pinMode(6, OUTPUT);
}

void loop() {
unsigned long res;

  Serial.print("Time: ");
  Serial.println(millis());
  Serial.println("Measuring...");

  digitalWrite(D6, HIGH);
  delayMicroseconds(10); 
  digitalWrite(D6, LOW);   
  res = pulseInLong(INPIN, HIGH, 3000000UL);


  if (res) {
     Serial.print("Time high (us): ");
     Serial.println(res);
  } else
     Serial.println("No signal present");

}
