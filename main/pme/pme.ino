/**
 * @file pme.ino
 * @author Jacob Valdez (valdez24@purdue.edu), Vincent Wang (wang5528@purdue.edu)
 * @brief The main sketch to control the Propellant Management Unit
 * @date 2023-10-22
 * 
 * @details The main datalogger runs this propellant management sketch to
 * determine the flight state, control the experiment, and log data onto an SD card. 
 * 
 */

// Set to 1 for simulated sensor inputs
#define SIM_MODE 1

#include "StateLogic.h"
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

// Writer parameters. Write to a binary file and choose to overwrite any existing files on the SD card
bool writeToBinary{true};
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
StateLogic statelogic;


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

  #if SIM_MODE
  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  poller.init("prof.csv");
  #else
  poller.init();
  #endif
  statelogic.init(0, 0);
}


void updateStateLogicMembers(float *vector) {
  int ret = poller.readVector(vector, millis());
  if (!ret) return;
  Serial.print("State{");
  for (int i = 0; i < 5; i++) {
    Serial.print(vector[i]);
    Serial.print(",");
  }
  Serial.println("}");
  statelogic.determineFlightState(millis(), vector);
  Serial.print(millis());
  Serial.print(" ");
  Serial.println(statelogic.flightState);
}


void loop() {
  float vector[5] = {0};
  pinController.Run();
  updateStateLogicMembers(vector);
  float pressures[5];
  float temperatures[5];
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

  logger.pushData(1.0f, vector, pressures, temperatures);
}
