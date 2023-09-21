#include <SD.h>
#include <SPI.h>

#include "src/Reader.h"
#include "src/AtmosphericStruct.h"

File dataFile;

const char* file_name{"LT02.DAT"};

union {
  float fval;
  byte bval[4];
} float2byte;

logType buf;

Reader reader(4);

void setup() {
    Serial.begin(115200);

    Serial.println("Start setup");
    if (reader.init() == 1) {
        while(1) {
            Serial.println("There is something wrong with the Reader class");
        }
    }

    if (!reader.readFile(file_name)) {
        while(1) {
            Serial.print("Couldn't read ");
            Serial.print(file_name);
            Serial.println(" from SD. Ensure this file exists and try again");
        }
    }

    buf = reader.readVector();
}

void loop() {
    delay(1000);
    for (int i=0; i < 100; ++i) {
        Serial.print(buf.time[i], 2);
        Serial.print('\t');
        Serial.print(buf.lowPressure[i],2);
        Serial.print('\t');
        Serial.print(buf.highPressure[i],2);
        Serial.print('\t');
        Serial.print(buf.acceleration[i],2);
        Serial.print('\n');
    }
}