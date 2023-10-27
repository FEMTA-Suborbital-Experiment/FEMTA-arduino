/**
 * @file flow_test.ino
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief A test sketch for propellant tank testing in a vacuum chamber
 * @date 2023-10-22
 * 
 * @details This sketch makes use of the statelogic class to handle the
 * control of the valves at certain pressure thresholds of the experiment.
 * Since a flow test does not have an acceleration component, we rely entirely
 * on the pressure sensors to determine flight state. 
 * 
 */

#include "PinCtrl.h"
#include "Logger.h"
#include "Writer.h"

const int closeSV1{A1}; 
const int openSV1{5};
const int closeSV2{A2};
const int openSV2{6};
const int closeLV1{A3}; 
const int openLV1{9};
const int signalPin{13};

const int bufferSize{50};

const char* fileName{"FT01"};

// Parameters for flight simulation
bool readSensors{true};

bool writeToBinary{true};
bool overwriteExistingFile{false};

// Define classes. Testing this to make sure the classes work
PinCtrl pinController(
  closeSV1, openSV1, closeSV2, openSV2, closeLV1, openLV1, signalPin
);
Logger logger(bufferSize);
Writer writer(fileName, 4, writeToBinary, overwriteExistingFile);

void setup() {
    if (pinController.init() != 0) {
        Serial.println("Something went wrong with initializing the pin controller. Exiting...");
        exit(1);
    }
    if (logger.init() != 0) {
        Serial.println("Something went wrong with initializing the logger. Exiting...");
        exit(1);
    }
    if (writer.init() != 0) {
        Serial.println("Something went wrong with initializing the writer. Exiting...");
        exit(1);
    }
}

void loop() {
    pinController.Run();
}