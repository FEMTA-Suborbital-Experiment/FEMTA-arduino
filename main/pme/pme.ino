// Include libraries
//#include "src/StateLogic.h"
#include "src/PinCtrl.h"
//#include <AtmosProfiles.h>
#include "src/Logger.h"

int closeSV1{A1}; 
int openSV1{5};
int closeSV2{A2};
int openSV2{6};
int closeLV1{A3}; 
int openLV1{9};
int signalPin{13};

// Parameters for flight simulation
bool readSensors{false};

// Define classes. Testing this to make sure the classes work
//StateLogic stateLogic(1000, 1000);
PinCtrl pinController(
  closeSV1, openSV1, closeSV2, openSV2, closeLV1, openLV1, signalPin
  );
//AtmosProfiles atmosProfiles;
Logger logger(100, readSensors, "LogTest");

void setup() {
  // put your setup code here, to run once:
/* 
  if (stateLogic.init()) {
    Serial.println("Something went wrong with initializing the state handler. Exiting...");
    exit(1);
  }; */
  if (pinController.init() != 0)
  {
    Serial.println("Something went wrong with initializing the pin controller. Exiting...");
    exit(1);
  }
  if (logger.init() != 0)
  {
    Serial.println("Something went wrong with initializing the logger. Exiting...");
    exit(1);
  }
}

void updateStateLogicMembers() {
  if (readSensors == true) {
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

// TODO: Once the classes are assembled and working, uncomment the appropriate sections
// and make sure they work
void loop() {
  // put your main code here, to run repeatedly:

  // Flight states:
  // 0: Preflight
  // 1: Liftoff/Ascent
  // 2: Zero-G
  // 3: Descent
  // 4: Landing

  /*
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
  */


  //atmosProfiles.advanceTimeStep();

  // Takes in an array of the current values. Just included a dummy value here
  //stateLogic.determineFlightState(1.0);
}
