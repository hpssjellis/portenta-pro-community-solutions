// Beginner C++ Programming String Input and Output
// You will need to open the Serial monitor and the word "Hi"

//Global Variables
String myIncoming = "";
int myLocation = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // I don't typically use the following
   while (!Serial) {};   // waits for the serial monitor to be attached

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enter something");
  while (Serial.available() == 0){} //waits until enter key pushed
  
  myIncoming = Serial.readString();
  Serial.println("You entered : " + String(myIncoming));

  //myIncoming.toUpperCase(); 
  myLocation = myIncoming.indexOf("Hi");  
  
  if(myLocation >= 0){  // -1 means nothing found
     Serial.println("You entered Hi at location: " + String(myLocation));
  } else {
     Serial.println("You did not enter Hi" );
  }

  myIncoming = "";
  Serial.println("----------------------------------");

  
}
