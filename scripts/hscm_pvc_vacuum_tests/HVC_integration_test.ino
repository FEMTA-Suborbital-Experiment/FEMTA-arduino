#include <PVC4000_101523.h> // substitute for the new one with temperature print ("t" at the end) (Pirani Code)
// put all data on the same file, it will be easier and the code will be more correct
#include <Wire.h>

#include <SPI.h>
#include <SD.h>

#include <HSCM.h> // HSCM code

// https://forum.arduino.cc/t/single-line-define-to-disable-code/636044
#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL if(DEBUG)Serial

PVC4000 pvc(0x50);
HSCM_PSI hscm(0x28, 0, 0);

const String fileName = "HVC06.TXT";

const int delayMS{100};
const int chipSelect{4}; 

String headers = "Time(s)";

float setupTime{0};

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pvc.init();

    while (!SD.begin(chipSelect)) {
        DEBUG_SERIAL.println("Card failed, or not present");
        // don't do anything more:
    }

    headers = headers + ", Pressure(PVC)(mtorr)";
    headers = headers + ", Temperature (ºC)";
    headers = headers + ", Pressure Raw Lower (count)";
    headers = headers + ", Pressure Raw Upper (count)";
    headers = headers + ", Pressure(HSCM)(psi)";
    headers = headers + ", Temperature (ºC)";

    File dataFile = SD.open(fileName, FILE_WRITE);
    if(dataFile) {
        DEBUG_SERIAL.println(headers);
        dataFile.println(headers);
        dataFile.close();
    }
    else {
        DEBUG_SERIAL.println("Error opening text file during setup");
    }

    Serial.begin(115200);
    Wire.begin();

    setupTime = millis() / 1000.0;
}

String appendData(String str, float input, uint8_t precision) {
  return str + String(input, precision) + ",";
}

void loop() {

    String dataString = "";

    float time = millis() / 1000.0 - setupTime;
    dataString = appendData(dataString, time, 3);
    
    int status = pvc.read();
    if (status == 0) {
        dataString = appendData(dataString, pvc.pressure(), 2);
        dataString = appendData(dataString, pvc.temperature(), 2);
        dataString = appendData(dataString, pvc.get_raw_lower(), 2);
        dataString = appendData(dataString, pvc.get_raw_upper(), 2);
    }

    int status_h = hscm.read();
    if (status_h < 2) {
        dataString = appendData(dataString, hscm.pressure(), 2);
        dataString = appendData(dataString, hscm.temperature(), 2);
    }
    else {
        DEBUG_SERIAL.println("Error reading from HSCM");
    }

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
    delay(delayMS); // check if we can take this out
}
