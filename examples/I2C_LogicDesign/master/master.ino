//this is the code for the board that is going to read values from the other two
//sensor 1 is accelleration data, 2 is temperature data

#include <Wire.h>
int state = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
//......int index = 0;
const int REMOTE = 8;
const int LOCAL = 4;
int size1 = 6;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // join I2C bus with address 4
  Serial.begin(9600); // 9600 bits per second
  Wire.onReceive(receiveEvent);
  
}

void loop() {
  Wire.requestFrom(REMOTE, 10*sizeof(char)); // 2nd param is exact size of thing being received
  while (Wire.available()){
    char c = Wire.read();
    Serial.print(c);
    //Serial.print("in while loop");
  }
  Serial.println(",");
  delay(1000);
  
}

void receiveEvent(int size1)
{
  Serial.println("receive event");
  while (Wire.available()){
    char c = Wire.read();
    Serial.print(c);
    //Serial.print("in while loop");
  }
  int x = Wire.read();
  Serial.print(x);
  Serial.println("is x value");
}
