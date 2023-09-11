#include "src/Reader.h"
#include "src/AtmosphericStruct.h"

File dataFile;

logType buf;

void setup() {
    File dataFile = open("LT01.DAT");
}

void loop() {
    if (dataFile.available()) {
        dataFile.read((uint8_t *)&buf.time, sizeof(buf.time));
        dataFile.read((uint8_t *)&buf.lowPressure, sizeof(buf.lowPressure));
        dataFile.read((uint8_t *)&buf.highPressure, sizeof(buf.highPressure));
        dataFile.read((uint8_t *)&buf.acceleration, sizeof(buf.acceleration));
    
        Serial.print(buf.ax, 6);
        Serial.print('\t');
        Serial.print(buf.ay,6);
        Serial.print('\t');
        Serial.print(buf.az,6);
        Serial.print('\t');
        Serial.print(buf.t,6);
        Serial.print('\n');
        
  }
}