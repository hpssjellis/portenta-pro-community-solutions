// classes for arduino are often put into libraries .h files
// we are going to do it here in the sketch

// define your class
class myTestClass {
public:
  int myA;
  boolean myB;
  void myReport() {
    Serial.println("value of myA: " + String(myA));
    Serial.println("value of myB: " + String(myB));
  }
};

// pass the class to a function
void myClassFunction(class myTestClass *myObject) {
  //(*myObject).myA += 1;
  //(*myObject).myReport();
    
  // OR use the easier arrow method
  myObject->myA +=  1;
  myObject->myB = !(myObject->myB);
  myObject->myReport();
}


// Global Variables define your object of the class
myTestClass myTestClassObject;

  
void setup() {
  Serial.begin(115200);
}


void loop() {   
  Serial.println("Loading the class");

  // call the function with your object
  myClassFunction(&myTestClassObject); 
  Serial.println("---------------------------");
  delay(3000);
  
}
