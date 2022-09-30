//this is the code for the board that is going to read values from the other two
//sensor 1 is accelleration data, 2 is temperature data

#include <Wire.h>
int state = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
int index = 0;


void setup() {
  Wire.begin();
  Serial.begin(9600); // 9600 bits per second
  
}

void loop() {
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.requestFrom(8, 6); // request 6 bytes from device #8
  
  while (Wire.available()) { // device may send less than requested
    char c = Wire.read(); // receive a byte as a character
    Serial.print(c); // print the character
  }
  delay(500);
  
   /*
  //this is just to make sure the board is looping and not stuck
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(10);                       // wait for a second

  
  //this is super rudimentary, going to make decisions based on data trends in final model
  if (accel[index] < 1 && Pressure[index] > 9 && Temperature[index] > 80){
    state = 0;
    Serial.print("pre-liftoff\n");
  }
  else if (accel[index] > 5 && Pressure[index] < 10 && Temperature[index] < 85){
    state = 1;
    Serial.print("ascent\n");
  }
  else if (accel[index] < 1 && Pressure[index] < 1 && Temperature[index] < 30){
    state = 2;
    Serial.print("MECO\n");
  }
  else if (accel[index] > 1 && Pressure[index] < 1 && Temperature[index] > 30){
    state = 3;
    Serial.print("descent\n");
  }
  else if (accel[index] < 1 && Pressure[index] > 9 && Temperature[index] < 80){
    state = 4;
    Serial.print("post-descent\n");
  }
  else{
    Serial.print("unsure of state\n");
  }
  index++;
  delay(1000);
  
  // */
}
