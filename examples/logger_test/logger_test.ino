#include "src/Logger.h"
#include "src/Writer.h"

// File name
const char* fileName{"LT09"};

// Write logged contents to binary
const bool writeToBinaryFile{1};

// Overwrite the existing log file during initialization
const bool overwriteExistingFile{1};

int chipSelect{4};

Logger logger(100);
Writer writer(fileName, chipSelect, writeToBinaryFile, overwriteExistingFile);
int increment{0};

void setup() {
    while (!Serial) {}

    Serial.begin(9600);
    
    if (logger.init() != 0) {
        Serial.println("Something went wrong with the logger. Exiting...");
        exit(1);
    }
    if (writer.init() != 0) {
        Serial.println("Something went wrong with the writer. Exiting...");
        exit(1);
    }
}

void loop() {
    delay(100);
    logger.pushData(1.1+increment, 2.1+increment, 3.1+increment, 4.1+increment);
    Serial.print("Time size: ");
    Serial.println(logger.logData.time.size());
    Serial.print("Low pressure size: ");
    Serial.println(logger.logData.lowPressure.size());
    Serial.print("High pressure size: ");
    Serial.println(logger.logData.highPressure.size());
    Serial.print("Acceleration size: ");
    Serial.println(logger.logData.acceleration.size());

    if (logger.isStructFilled()) {
        writer.writeToFile(logger.logData);
        logger.flushArrays();
    }
    increment++;
}