/**
 * @file flow_test.ino
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief A test sketch for propellant tank testing in a vacuum chamber
 * @date 2023-10-22
 * 
 * @details This sketch of a custom function to handle the
 * control of the valves at certain pressure thresholds of the experiment.
 * Since a flow test does not have an acceleration component, we rely entirely
 * on the pressure sensors to determine flight state. In addition, we also provide
 * a simulation mode for offline testing. 
 * 
 */// Set to 1 for simulated sensor inputs
#define SIM_MODE 0

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
const char* fileName{"FT21"};

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

unsigned long lastRead{0};
unsigned long startTime{0};

void setup() {
    Serial.println("Initializing pin controller");
    if (pinController.init() != 0) {
        Serial.println("Something went wrong with initializing the pin controller. Exiting...");
        exit(1);
    }

    Serial.println("Initializing logger");
    if (logger.init() != 0) {
        Serial.println("Something went wrong with initializing the logger. Exiting...");
        exit(1);
    }

    Serial.println("Initializing writer");
    if (writer.init() != 0) {
        Serial.println("Something went wrong with initializing the writer. Exiting...");
        exit(1);
    }

    Serial.println("Initializing sensor poller");

    #if SIM_MODE
    poller.init("prof.csv");
    #else
    poller.init();
    #endif
    unsigned long lastRead = millis();
    unsigned long startTime = millis();
}

/**
 * @brief Custom function that the user can write to the specifications of a specific test.
 * 
 * @details You can replicate the behavior of pme.ino by placing the flight state determination
 * algorithm in this function. 
 * 
 * @param ambient 
 * @param pressures 
 * @param temperatures 
 */
void controlExperiment(float *ambient, float *pressures, float *temperatures, float *flow) {
    Serial.println("");
    Serial.print("Ambient State: [ ");
    for (int i = 0; i < 5; ++i) {
        Serial.print(String(ambient[i]));
        if (i < 4)
            Serial.print(", ");
    }
    Serial.println("]");

    Serial.print("Tank Pressures: [ ");
    for (int i = 0; i < 5; ++i) {
        Serial.print(String(pressures[i]));
        if (i < 4)
            Serial.print(", ");
    }
    Serial.println("]");

    Serial.print("Tank Temperatures: [ ");
    for (int i = 0; i < 5; ++i) {
        Serial.print(String(temperatures[i]));
        if (i < 4)
            Serial.print(", ");
    }
    Serial.println("]");

    Serial.print("Flow State: [ ");
    for (int i = 0; i < 4; ++i) {
        Serial.print(String(flow[i]));
        if (i < 3)
            Serial.print(", ");
    }
    Serial.println("]");
}

void loop() {
    pinController.Run();
    currentRead = millis();
//    if ((currentRead - lastRead) > (1000.0 / poller.pollRate)) {
//        lastRead = currentRead;
        
        float flow[4] = {0};
        float ambient[5] = {0};
        float pressures[5] = {0};
        float temperatures[5] = {0};
        
        // FIXME: Poller not reading HSCM
        int ret = poller.readVector(ambient, currentRead);
        if (ret == 1) {
            poller.readPressureSensors(pressures, temperatures);
            poller.readFlowMeter(flow);

            Serial.println("Read data");
            controlExperiment(ambient, pressures, temperatures, flow);
        }

        // FIXME: Flow data not being pushed into logger.
        logger.pushData(currentRead - startTime, ambient, pressures, temperatures, flow);
//    }

    if (logger.isStructFilled()) {
        writer.writeToFile(logger.logData);
        logger.flushArrays();
    }
}