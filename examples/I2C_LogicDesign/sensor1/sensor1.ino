//code for sensor board 1

//logic: get signal from master, send one value, queue next value, wait for next signal
#include <Wire.h>
int accel[30] = { 0, 1, 1, 1, 1, 5, 4, 9, 9, 9, 7, 7, 7, 7, 8, 4, 2, 0, 0, 2, 3, 4, 4, 3, 3, 4, 6, 9, 4, 0}; // mock acceleration data
float Pressure[30] = {101250, 101250, 101250, 101000, 100500, 100000, 99000, 98000, 95000, 90000, 85000, 75000, 50000, 40000, 30000, 15000, 5000, 500, 50, 1, 0.2, 0.08, 0.001, 0.0005, 0.0005, 0.0005, 0.0005, 0.08, 0.5, 500}; // mock pressure data
int x = 0;
String stringData = " ";
const int LOCAL = 8;
const int REMOTE = 4;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(LOCAL); // join I2C bus with address 8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  

}

void loop() {
  /*
  stringData = String(Pressure[x], 5);
  for (int i = 0; i < stringData.length(); i++){
    Serial.print(stringData[i]); 
  }
  Serial.print(" and string length is ");
  Serial.println(stringData.length());
  //Serial.println(x);
  x++;
  if(x >= (sizeof(Pressure) / sizeof(Pressure[0]))){
    x = 0;
  }//*/
  delay(100);
}

//function that executes whenever data is requested by master, called an event
///*
void requestEvent() {
  stringData = "";
  stringData.concat(String(Pressure[x], 5)); // 5 is number of decimal places to include
  stringData.concat(",");
  stringData.concat(accel[x]);
  stringData.concat(";");
  for (int i = 0; i < stringData.length(); i++){
    Wire.write(stringData[i]); 
  }
  x++;
  if(x >= (sizeof(Pressure) / sizeof(Pressure[0]))){
    x = 0;
  }
}
//*/
