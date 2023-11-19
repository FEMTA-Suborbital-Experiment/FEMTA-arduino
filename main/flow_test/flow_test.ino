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
 */// Set to 1 for simulated sensor inputs
#define SIM_MODE 1

#include "PinCtrl.h"
#include "Logger.h"
#include "Writer.h"
#if SIM_MODE
#include "SensorPollerFake.h"
#else
#include "SensorPoller.h"
#endif

// Pin Mappings to Opening and Closing Valves
const int closeSV1{A1}; 
const int openSV1{5};
const int closeSV2{A2};
const int openSV2{6};
const int closeLV1{A3}; 
const int openLV1{9};
const int signalPin{13};

// Buffer size to store data in the logger
const int bufferSize{50};

// Name of the output data file
const char* fileName{"FT01"};

bool writeToBinary{false};
bool overwriteExistingFile{false};

// Define classes. Testing this to make sure the classes work
PinCtrl pinController(
  closeSV1, openSV1, closeSV2, openSV2, closeLV1, openLV1, signalPin
);
Logger logger(bufferSize);
Writer writer(fileName, 4, writeToBinary, overwriteExistingFile);
#if SIM_MODE
SensorPollerFake poller;
#else
SensorPoller poller;
#endif

float pressure[5];
float temperature[5];

int lastTime;

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

    #if SIM_MODE
    poller.init("/sim_profile.csv");
    #else
    poller.init();
    #endif
    lastTime = millis();
}

void loop() {
    pinController.Run();
    readTime = millis();
    if (readTime - lastTime) > (1000 / poller.pollRate) {
        t = readTime - lastTime;
        lastTime = readTime;
        poller.readPressureSensors(pressure, temperature);
        logger.pushData(t, 0, 0, 0, pressure, temperature);
    }

    if (logger.isStructFilled()) {
        writer.writeToFile(logger.logData);
        logger.flushArrays();
    }
}