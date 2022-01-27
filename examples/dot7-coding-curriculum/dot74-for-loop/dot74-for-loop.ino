
// Global Variables
int myCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  myCount += 1; 
  Serial.println("myCount: " + String(myCount) );
  for (int myLoop=0; myLoop <= myCount; myLoop++){
    Serial.print(myLoop);  
    Serial.print(", ");  
  }
  Serial.println();
  Serial.println("-----------------------------" );
  Serial.println();
  delay(5000);

}
