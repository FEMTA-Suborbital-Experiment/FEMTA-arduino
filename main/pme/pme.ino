/**
 * @file pme.ino
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief The main sketch to control the Propellant Management Unit
 * @date 2023-10-22
 * 
 * @details The main datalogger runs this propellant management sketch to
 * determine the flight state, control the experiment, and log data onto an SD card. 
 * 
 */

#include "SensorPoller.h"
// #include "SensorPollerFake.h
#include "StateLogic.h"
#include "PinCtrl.h"
#include "Logger.h"
#include "Writer.h"

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

// Determine if we will read from the sensors or from a csv file
bool readSensors{true};

// Writer parameters. Write to a binary file and choose to overwrite any existing files on the SD card
bool writeToBinary{true};
bool overwriteExistingFile{false};

// Define classes. Testing this to make sure the classes work
PinCtrl pinController(
  closeSV1, openSV1, closeSV2, openSV2, closeLV1, openLV1, signalPin
);
Logger logger(bufferSize);
Writer writer(fileName, 4, writeToBinary, overwriteExistingFile);


void setup() {
  // if (stateLogic.init()) {
  //   Serial.println("Something went wrong with initializing the state handler. Exiting...");
  //   exit(1);
  // }
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


void updateStateLogicMembers() {
  if (readSensors) {
    // Update state logic members to get sensor values
  } else {
  // TODO: Implement atmospheric profiles first
  /* 
    int currentTimeStep = atmosProfile.timeStep
    stateLogic.lowVacuumPressure = atmosProfiles.pressureProfile[currentTimeStep];
    stateLogic.highVacuumPressure = atmosProfiles.pressureProfile[currentTimeStep];
    stateLogic.acceleration = atmosProfile.accelerationProfile[currentTimeStep];
 */
  }
}


void loop() {
  pinController.Run();


  // switch(stateLogic.flightState) {
  //   case 0:
  //   case 1:
  //     pinController.closeFlowValve();
  //     pinController.openVentValve();
  //     break;
  //   case 2:
  //     pinController.signalStart();
  //     pinController.openFlowValve();
  //     pinController.closeVentValve();
  //   case 3:
  //     pinController.signalStop();
  //     pinController.closeFlowValve();
  //     pinController.closeVentValve();
  // }

  logger.pushData(1.0f, 0.0, 0.0, 0.0);
}
