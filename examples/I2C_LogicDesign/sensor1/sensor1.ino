//code for sensor board 1

//logic: get signal from master, send one value, queue next value, wait for next signal
#include <Wire.h>
#include <CSV_Parser.h>
#include <SPI.h>
#include <SD.h>

float* acceleration;
float* pressure;
int x = 0; // iterator
int rowcount = 0;
String stringData = "";
const int LOCAL = 8;
const int REMOTE = 4;
const int chipSelect = 4;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(LOCAL); // join I2C bus with address 8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  Serial.println("Initializing card");
  if(!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    
    // don't do anything more:
    while (1);
  }
  Serial.println("Card initialized");
  
  CSV_Parser cp(/*format*/ "ffff", /*has_header*/ true, /*delimiter*/ ',');
  
  if(cp.readSDfile("FSDO.CSV")) {
    //float *altitude = (float*)cp["altitude"];
    //float *temperature = (float*)cp["temperature"];
    pressure = (float*)cp["pressure"];
    acceleration = (float*)cp["acceleration"];
    rowcount = cp.getRowsCount();
  }
}

void loop() {

  delay(100);
}

//function that executes whenever data is requested by master, called an event
///*
void requestEvent() {
  stringData = "";
  stringData.concat(String(pressure[x], 5)); // 5 is number of decimal places to include
  stringData.concat(",");
  stringData.concat(acceleration[x]);
  stringData.concat(";");
  for (int i = 0; i < stringData.length(); i++){
    Wire.write(stringData[i]); 
  }
  x++;
  if(x >= rowcount){
    x = 0;
  }
}
//*/
