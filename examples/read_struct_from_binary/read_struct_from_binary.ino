#include "src/Reader.h"
#include "src/AtmosphericStruct.h"

File dataFile;

typedef struct {
    float time;
    float lowPressure;
    float highPressure;
    float acceleration;
}  buf;

void setup() {
    File dataFile = open("LT01.DAT");
}

void loop() {
    if (dataFile.available()) {
        dataFile.read((uint8_t *)&buf.time, sizeof(buf.time));
        dataFile.read((uint8_t *)&buf.lowPressure, sizeof(buf.lowPressure));
        dataFile.read((uint8_t *)&buf.highPressure, sizeof(buf.highPressure));
        dataFile.read((uint8_t *)&buf.acceleration, sizeof(buf.acceleration));
    
        Serial.print(buf.time, 6);
        Serial.print('\t');
        Serial.print(buf.lowPressure,6);
        Serial.print('\t');
        Serial.print(buf.highPressure,6);
        Serial.print('\t');
        Serial.print(buf.acceleration,6);
        Serial.print('\n');
  }
}