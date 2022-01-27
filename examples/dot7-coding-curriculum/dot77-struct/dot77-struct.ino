// Global Variables

// A STRUCT is like an object or class in other computer languages 
// You could make an array of the struct

struct STRUCT {
  int myCount;
  float myStructFloat;
} myTestStruct;

void setup() {
  Serial.begin(115200);
  randomSeed( analogRead(A1) );  // set the random generator to A1 a random starting number
  
}

void loop() {

  myTestStruct.myCount += 1;
  myTestStruct.myStructFloat = random(0,10000);

  Serial.println("Counting myTestStruct.myCount: " + String(myTestStruct.myCount));
  Serial.println("Random myTestStruct.myStructFloat: " + String(myTestStruct.myStructFloat));
  Serial.println("-----------------------------" );
  Serial.println();
  delay(2000);

}
