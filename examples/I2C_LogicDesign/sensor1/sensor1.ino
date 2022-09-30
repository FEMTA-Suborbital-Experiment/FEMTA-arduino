//code for sensor board 1

//logic: get signal from master, send one value, queue next value, wait for next signal
#include <Wire.h>
int accel[25] = { 0, 4, 9, 9, 9, 7, 7, 7, 7, 8, 4, 2, 0, 0, 2, 3, 4, 4, 3, 3, 4, 6, 9, 4, 0}; // mock acceleration data
void setup() {
  Wire.begin(8); // join I2C bus with address 8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);

}

void loop() {
  
  delay(100);
}

void receiveEvent(){
  Wire.
}
//function that executes whenever data is requested by master, called an event
void requestEvent() {
  Wire.write("hello");//6 byte message is sent
}
