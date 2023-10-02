#include <SD.h>
#include <SPI.h>

#include "src/Reader.h"
#include "src/AtmosphericStruct.h"

File dataFile;

const char* file_name{"LT06.DAT"};

union {
  float fval;
  byte bval[4];
} float2byte;

logType buf;

Reader reader(4);

void setup() {
    Serial.begin(9600);
    delay(3000);
    Serial.println("Start setup");
    if (reader.init() == 1) {
        while(1) {
            Serial.println("There is something wrong with the Reader class");
        }
    }
    Serial.println("Initialized reader");

    if (!reader.readFile(file_name)) {
        while(1) {
            Serial.print("Couldn't read ");
            Serial.print(file_name);
            Serial.println(" from SD. Ensure this file exists and try again");
        }
    }
    Serial.println("Read file");

    buf = reader.readVector();
    Serial.println("Finish setup");
}

/**
 * @brief Reads the output from logger_test.ino and outputs it to Serial.
 * Preserves floats!
 * 
 */
void loop() {
    Serial.println("Reading vector");
    delay(1000);
    for (int i=0; i < 300; ++i) {
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