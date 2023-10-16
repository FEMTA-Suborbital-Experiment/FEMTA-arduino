#include <PVC4000.h>

#include <Wire.h>

#include <SPI.h>
#include <SD.h>

// https://forum.arduino.cc/t/single-line-define-to-disable-code/636044
#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL if(DEBUG)Serial

PVC4000 pvc(0x50);

const bool isLogging{0};
const String fileName = "PVC05.TXT";

const int delayMS{100};
const int chipSelect{4}; 

String headers = "Time(s)";

int startupTime{0};

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pvc.init();


    if (isLogging) {
        while (!SD.begin(chipSelect)) {
            DEBUG_SERIAL.println("Card failed, or not present");
            // don't do anything more:
        }

        headers = headers + ", Pressure(PVC)(mtorr)";
        headers = headers + ", Pressure Raw Lower (count)";
        headers = headers + ", Pressure Raw Upper (count)";

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
    startupTime = millis();
}

String appendData(String str, float input, uint8_t precision) {
  return str + String(input, precision) + ",";
}


void loop() {

    String dataString = "";

    float time = (millis() - startupTime) / 1000.0;
    dataString = appendData(dataString, time, 3);
    
    int status = pvc.read();
    if (status == 0) {
        dataString = appendData(dataString, pvc.pressure(), 2);
        dataString = appendData(dataString, pvc.temperature(), 2);
        dataString = appendData(dataString, pvc.baselineTemperature(), 2);
        //dataString = appendData(dataString, pvc.get_raw_lower(), 2);
        //dataString = appendData(dataString, pvc.get_raw_upper(), 2);
    }
    
    DEBUG_SERIAL.println(dataString);
     
    if (isLogging) {
        File dataFile = SD.open(fileName, FILE_WRITE);

        // if the file is available, write to it:
        if (dataFile) {
            dataFile.println(dataString);
            dataFile.close();
            // print to the serial port too:
        }
        // if the file isn't open, pop up an error:
        else {
            DEBUG_SERIAL.println("error opening datalog.txt");
        }
    }
    delay(delayMS);
}
