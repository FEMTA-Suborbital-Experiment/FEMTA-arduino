#include <sensirion-lf.h>

#include <Wire.h>

#include <SPI.h>
#include <SD.h>

// https://forum.arduino.cc/t/single-line-define-to-disable-code/636044
#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL if(DEBUG)Serial

const String fileName = "FT04.TXT";

const int delayMS{100};
const int chipSelect{4}; 

// Set these to true if flow measurement or pressure measurements or both are expected
boolean canMeasureFlow{true};

float setupTime{};

String headers = "Time(s)";

void setup() {
    
    DEBUG_SERIAL.begin(115200);

    DEBUG_SERIAL.println("Begin Setup");


    Wire.begin();

    while (!SD.begin(chipSelect)) {
        DEBUG_SERIAL.println("Card failed, or not present");
        // don't do anything more:
    }

    if (SLF3X.init() == 1) {
        DEBUG_SERIAL.println("Error during SLF3X init.");
        canMeasureFlow = false;
    }
    else {
        headers = headers + ", Flowrate(ml/min), Temperature(C), AirInLine, HighFlowDetected";
        canMeasureFlow = true;
    }

    File dataFile = SD.open(fileName, FILE_WRITE);
    if(dataFile) {
        DEBUG_SERIAL.println(headers);
        dataFile.println(headers);
        dataFile.close();
    }
    else {
        DEBUG_SERIAL.println("Error opening text file during setup");
    }

    setupTime = millis() / 1000.0;
}


String appendData(String str, float input) {
  return str + String(input) + ",";
}


void loop() {

  String dataString = "";

  float time = millis() / 1000.0 - setupTime;
  dataString = appendData(dataString, time);
  
  if (canMeasureFlow) {
    int ret = SLF3X.readSample();
    if (ret == 0) {
        dataString = appendData(dataString, SLF3X.getFlow());
        dataString = appendData(dataString, SLF3X.getTemp());
        dataString = appendData(dataString, SLF3X.isAirInLineDetected());
        dataString = appendData(dataString, SLF3X.isHighFlowDetected());
    }
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