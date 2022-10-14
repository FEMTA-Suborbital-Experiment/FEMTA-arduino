//this is the code for the board that is going to read values from the other two
//sensor 1 is accelleration data, 2 is temperature data

#include <Wire.h>
int state = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
int index = 0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600); // 9600 bits per second
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.requestFrom(8, 1); // request 1 byte from device #8
  
  //Serial.print("in void loop\n");
  while (Wire.available()) { // device may send less than requested
    int c = Wire.read(); // receive a byte as an int
    Serial.print(c); // print the int
    Serial.print(" is accel data\n");
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
