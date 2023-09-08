#include "src/Logger.h"

Logger logger(100);

void setup() {
    if (logger.init() != 0) {
        Serial.println("Something went wrong with the logger. Exiting...");
        exit(1);
    }
}

void loop() {
    logger.pushData(1, 1, 1, 1);
    Serial.println(logger.logData.time.size());
    Serial.println(logger.logData.lowPressure.size());
    Serial.println(logger.logData.highPressure.size());
    Serial.println(logger.logData.acceleration.size());
}