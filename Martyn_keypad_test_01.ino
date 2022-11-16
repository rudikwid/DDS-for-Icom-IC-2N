/*
Keypad Tester 01. Created by Martyn Currey
*/

// Global Variables
const byte keypadPin     = A2;
int val = 0;

void setup() 
{
  Serial.begin(9600); 
  while (!Serial)   {  ;    }
  Serial.println(F("Start\n"));
  // set the analogue pin for input and turn on the internal pullup resistor
  pinMode(keypadPin, INPUT_PULLUP); 
}

void loop()
{
    val = getKeyValue();
    Serial.println( val );
    delay(100);
} 


int getKeyValue()
{
    int pinVal;
    pinVal = analogRead(keypadPin);
    return pinVal;
}
