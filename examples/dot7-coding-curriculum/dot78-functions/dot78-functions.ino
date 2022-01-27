
void mySimple(){ 
  Serial.println("Now inside the simple function");
}

int myAdd(int x, int y){ 
  int myAdded = x + y;
  return myAdded;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Starting the main loop");
  mySimple();
  
  int myX = random(5,8);
  int myY = random(2,4); 
  
  Serial.println("Now passing variables to the myAdd function to return an answer");
  int myAnswer = myAdd(myX, myY);
  
  Serial.println("Adding myX: "+ String(myX) + " and myY: " + String(myY) + " = " + String(myAnswer));
  Serial.println("------------------------------");
  delay(2000); 
}

