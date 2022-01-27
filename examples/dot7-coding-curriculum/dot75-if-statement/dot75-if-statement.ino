// Global Variables
int myCount = 0;

void setup() {
  Serial.begin(115200);
  
  // I don't like the following
  while (!Serial){} // blocks the code until the serial monitor is loaded
  // but when you code without the serial monitor your code does not work!
}


void loop() {
  myCount += 1;
  Serial.println("myCount: " + String(myCount) );
  
  if (myCount == 7){   //  == is a question
     Serial.println("I love the number 7" );
  }
  
  if (myCount % 2 == 0){   // remainder after dividing by 2 
     Serial.println("An Even number!" );
  } else {
      Serial.println("Odd number!" );   
  }
  
  if (myCount >= 3 && myCount <= 6){    // AND
     Serial.println("Between 3 and 6" );
  }
  
  if (myCount == 9 || myCount == 11){    // OR
     Serial.println("9's and 11's are great." );
  }

  Serial.println("-------------------------------" );
  delay(2000);
  
}
