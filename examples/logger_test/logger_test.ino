#include "src/Logger.h"
#include "src/Writer.h"

Logger logger(100);
Writer writer("LT01", 0);

void setup() {
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
    delay(500);
    logger.pushData(1, 1, 1, 1);
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
}