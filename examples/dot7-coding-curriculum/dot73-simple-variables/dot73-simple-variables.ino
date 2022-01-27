// refer to 
// https://www.arduino.cc/reference/en/#variables


// Global Variables
bool myBool01 = false;
int myInt01 = 34;
long myLong01 = 23452345;
float myFloat01 = 34.23;
char myChar01 = 'F';
String myString = "Fred";
// many more types of variables including advanced arrays and structs

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  int myLocal = 5;
}

void loop() {
  // put your main code here, to run repeatedly:
  int myLocal = 99;
  myInt01 += 1;
  myFloat01 = myFloat01 * myInt01
  Serial.println("Counting myInt01:   " + String(myInt01) );
  Serial.println("myBool01:  " + String(myBool01) );
  Serial.println("myLong01:  " + String(myLong01) );
  Serial.println("myFloat01: " + String(myFloat01,3) );
  Serial.println("myChar01:  " + String(myChar01) );
  Serial.println("myString:  " + myString );  
  Serial.println("Why doesn't myLocal = 5 when it equals: " + String(myLocal) );

  Serial.println("------------------------------------");
  Serial.println();
  delay(5000);
}
