/*  
*  FSLP   Force sensor linear Potentiometer Pololu #2730 
* https://www.pololu.com/product/2730 
* 
* looking at the strip the three pins are: 
* ______________________________________--- D1    PIN_REFERENCE 
* ......  .........   ........   .......--- A5     PIN_WIPER
* ______________________________________--- A4
*  resistor attached D0 connected parallel with A4    PIN_RIGHT
* 
*/



#define SERIAL_BAUD_RATE    115200

#define PER_CYCLE_DELAY     25

#define TOUCH_THRESH        25





/**********************************************************************************************************

* GLOBALS

**********************************************************************************************************/




/**********************************************************************************************************

* setup()

**********************************************************************************************************/

void setup()

{

  Serial.begin(SERIAL_BAUD_RATE);

  

  pinMode(D0, OUTPUT);

  pinMode(D1, INPUT);

  pinMode(A5, INPUT);
  pinMode(A4, INPUT);

}





/**********************************************************************************************************

* loop()

**********************************************************************************************************/

void loop() 

{

  /*** First read force ***/

  

  pinMode(D1, OUTPUT);

  digitalWrite(D1, LOW);  //Ground reference divider resistor

  digitalWrite(D0, HIGH);     //Both ends of pot are at +5V

  delay(1);

  int force_reading = analogRead(A5);
  int pos_reading = analogRead(A4);

  

  

  /*** Now read position ***/

  

  pinMode(D1, INPUT);     //Effectively disconnect reference divider resistor

  digitalWrite(D0, LOW);      //Left end of pot is at +5V, Right end is at GND

  delay(1);

  int position_reading = 1023 - analogRead(A5);  //subtract to output position relative to Left end 

  position_reading -= 512;           //subtract half-scale reading; sensor center is position 0, left is -512, right is +512

  

  

  /*** If readings are valid, output position and force ***/

  

  if(force_reading >= TOUCH_THRESH)  //With no force on the sensor the wiper is floating, position readings would be bogus

  {

    Serial.print("Position: ");

    printFixed(position_reading, 3, true);

    Serial.print("Posit: ");

    printFixed(pos_reading, 3, true);

    Serial.print("   Force: ");

    printFixed(force_reading, 3, false);

    Serial.println();

  }

  else

  {

    Serial.println("Position:        Force:   0");

  }

  

  delay(PER_CYCLE_DELAY);

}





/**********************************************************************************************************

* printFixed() - Serial Print with space padding for consistent positioning in terminal. Optional +/- sign

**********************************************************************************************************/

void printFixed(int value, int digit_places, boolean show_sign)

{

  if(value < 0)

  {

    Serial.print("-");

    value = abs(value);

  }

  else if(show_sign)

  {

    Serial.print("+");

  }

  int compare = 10;

  for(int i = 0; i < digit_places - 1; i ++)

  {

    if(value < compare)

    {

      Serial.print(" ");

    }

    compare *= 10;

  } 

  Serial.print(value);
  delay(600);

}
