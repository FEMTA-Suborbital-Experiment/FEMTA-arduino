#include "../../main/pme/src/StateLogic.h"
#include "../../main/pme/src/AtmSphericProf.h"

void setup() {
  // put your setup code here, to run once:
  const char* test_csv_file = "../flight-profile/acceleration_liftoff_MECO.csv";
  AtomSphericProfile data(test_csv_file);
  data.ParseCSV();

  // Initialize state logic
  StateLogic stateLogic;
  stateLogic.init();
  int i = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  float sensorArray[5] = {0};
  sensorArray[0] = data.accel_x[i];
  // sensorArray[3] = data.pressure_hscm[i];
  // sensorArray[4] = data.pressure_mpi[i];
  // sensorArray[0] = data.accel_x[i];
  // sensorArray[1] = data.accel_y[i];
  // sensorArray[2] = data.accel_z[i];
  global_time = data.time[i] / 10;

  int state = stateLogic.determineFlightState(millis(), sensorArray);
  if (i < data.count) {
    printf("Flight state at %lu: %lf %d\n", millis(), sensorArray[0], state);
    i += 1;
  }
}
