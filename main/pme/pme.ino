// Include libraries
#include "src/SensorPoller.h"
#include "src/StateLogic.h"
#include "src/PinCtrl.h"
//#include <AtmosProfiles.h>
//#include <Logger.h>

// Include sensor libraries


// Define classes
StateLogic stateLogic;
PinCtrl pinController;
AtmosProfiles atmosProfiles;
Logger logger;

// Parameters for flight simulation
bool readSensors{false};

// Record last sensor read time
unsigned long lastRead = 0;

void setup() {
  // put your setup code here, to run once:

  stateLogic.init();
  pinController.init();

  if (readSensors == true) {
    // Initialize all sensors
  }
  else {
    atmosProfiles.init()
  }

  // Configure logger
  logger.transformToStruct(false);

}

void updateStateLogicMembers() {
  if (readSensors == true) {
    // Update state logic members to get sensor values
  } else {
    int currentTimeStep = atmosProfile.timeStep
    stateLogic.lowVacuumPressure = atmosProfiles.pressureProfile[currentTimeStep];
    stateLogic.highVacuumPressure = atmosProfiles.pressureProfile[currentTimeStep];
    stateLogic.acceleration = atmosProfile.accelerationProfile[currentTimeStep];
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // Flight states:
  // 0: Preflight
  // 1: Liftoff/Ascent
  // 2: Zero-G
  // 3: Descent
  // 4: Landing

  switch(stateLogic.flightState) {
    case 1:
      pinController.closeFlowValve();
      pinController.openVentValve();
      break;
    case 2:
      pinController.signalStart();
      pinController.openFlowValve();
      pinController.closeVentValve();
    case 3:
      pinController.signalStop();
      pinController.closeFlowValve();
      pinController.closeVentValve();
  }

  atmosProfiles.advanceTimeStep();
  stateLogic.determineFlightState();

  if ((time_millis - lastRead) > 1000 / (pollRate)) {
  }
}
