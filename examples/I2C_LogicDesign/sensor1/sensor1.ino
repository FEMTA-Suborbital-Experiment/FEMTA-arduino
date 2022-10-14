//code for sensor board 1

//logic: get signal from master, send one value, queue next value, wait for next signal
#include <Wire.h>
int accel[25] = { 0, 4, 9, 9, 9, 7, 7, 7, 7, 8, 4, 2, 0, 0, 2, 3, 4, 4, 3, 3, 4, 6, 9, 4, 0}; // mock acceleration data
int x = 0;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(8); // join I2C bus with address 8
  Wire.onRequest(requestEvent); // register event
  //Serial.print(sizeof(accel[0]));
  //Serial.print("is size of one elem");
  

}

void loop() {
  delay(100);
}

//function that executes whenever data is requested by master, called an event
void requestEvent() {
  Wire.write(accel[x]);//2 byte message is sent
  x++;
  if(x >= (sizeof(accel) / sizeof(accel[0]))){
    x = 0;
  }
}
