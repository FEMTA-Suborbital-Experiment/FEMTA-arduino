#include <sensirion-lf.h>
#include <MS5837.h>

#include <Wire.h>

#include <SPI.h>
#include <SD.h>

const int delayMS{100};
const int chipSelect{4}; 

MS5837 sensor;

String headers = "Time(s), Pressure(mbar), Flowrate(ml/min), Temperature(C), AirInLine, HighFlowDetected";

void setup() {
    
    Serial.begin(115200); // initialize serial communication

    Serial.println("Begin Setup");

    Wire.begin();

    while (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
    }
    /*
    while (SLF3X.init() == 0) {
        Serial.println("Error during SLF3X init. Stopping application.");
        delay(1000); // loop forever
    }
    */
    /*
    while (!sensor.init()) {
        Serial.println("Init failed!");
        Serial.println("Are SDA/SCL connected correctly?");
        Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
        Serial.println("\n\n\n");
        delay(5000);
    }
    
    sensor.setModel(MS5837::MS5837_02BA);
    sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
    */
    File dataFile = SD.open("FT02.txt", FILE_WRITE);
    if(dataFile) {
        dataFile.println(headers);
        dataFile.close();
    }
    else {
        Serial.println("Error opening text file during setup");
    }
}


String appendData(String str, float input) {
  return str + String(input) + ",";
}


void loop() {

  String dataString = "";

  float time = millis() / 1000.0;
  // TODO: Function to append to dataString
  dataString = appendData(dataString, time);
  dataString = appendData(dataString, sensor.pressure());
  // dataString = appendData(dataString, );

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("FT02.txt", FILE_WRITE);

  // if the file is available, write to it:
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
        Serial.println("error opening datalog.txt");
    }
    delay(delayMS);
}