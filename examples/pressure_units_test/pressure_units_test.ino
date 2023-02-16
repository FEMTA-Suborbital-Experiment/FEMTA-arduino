#include <sensirion-lf.h>
#include <MS5837.h>
#include <HSCM.h>

#include <Wire.h>

#include <SPI.h>
#include <SD.h>

// https://forum.arduino.cc/t/single-line-define-to-disable-code/636044
#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL if(DEBUG)Serial

const String fileName = "FT06.TXT";

const int delayMS{100};
const int chipSelect{4}; 

// Set these to true if pressure measurements or both are expected
boolean canMeasurePressure{false};

MS5837 sensor;
HSCM_PSI hscm(0x28, 0, 0);

String headers = "Time(s)";

void setup() {
    
    DEBUG_SERIAL.begin(115200);

    DEBUG_SERIAL.println("Begin Setup");


    Wire.begin();

    while (!SD.begin(chipSelect)) {
        DEBUG_SERIAL.println("Card failed, or not present");
        // don't do anything more:
    }

    if (!sensor.init()) {
        DEBUG_SERIAL.println("Init failed!");
        DEBUG_SERIAL.println("Are SDA/SCL connected correctly?");
        DEBUG_SERIAL.println("Blue Robotics Bar30: White=SDA, Green=SCL");
        DEBUG_SERIAL.println("\n\n\n");
        canMeasurePressure = false;
    }
    else {
        canMeasurePressure = true;
        headers = headers + ", Pressure(Prop)(mbar)";
        headers = headers + ", Pressure(HSCM)(psi)";
        DEBUG_SERIAL.println("Pressure sensor initialized");
    }

    sensor.setModel(MS5837::MS5837_02BA);
    sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

    File dataFile = SD.open(fileName, FILE_WRITE);
    if(dataFile) {
        DEBUG_SERIAL.println(headers);
        dataFile.println(headers);
        dataFile.close();
    }
    else {
        DEBUG_SERIAL.println("Error opening text file during setup");
    }
}


String appendData(String str, float input, uint8_t precision) {
  return str + String(input, precision) + ",";
}


void loop() {

  String dataString = "";

  float time = millis() / 1000.0;
  dataString = appendData(dataString, time, 3);
  
  if (canMeasurePressure) {
    sensor.read();
    int status = hscm.read();
    dataString = appendData(dataString, sensor.pressure(), 4);
    dataString = appendData(dataString, hscm.pressure(), 8);
  }

 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        DEBUG_SERIAL.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
        DEBUG_SERIAL.println("error opening datalog.txt");
    }
    delay(delayMS);
}