/**
 * This test file initializes a StateLogic manager and
 * feeds it data from an example flight profile. The test
 * checks to see if the state logic transitions to the correct
 * stages at the correct times.
 *
 * TODO: Port this to a testing framework such as AUnit
 */
#include "Arduino.h"
#include <iostream>
#include <stdio.h>
#include "../../main/pme/src/StateLogic.h"
#include "../../main/pme/src/AtmSphericProf.h"

double global_time = 0;

unsigned long millis() {
    return global_time * 1000;
}


int main(int argc, char **argv) {
    // Read in data
    const char* test_csv_file = "../flight-profile/acceleration_full_metric.csv";
    AtomSphericProfile data(test_csv_file);
    data.ParseCSV();

    // Initialize state logic
    StateLogic stateLogic;
    stateLogic.init();

    for (int i = 0; i < data.count; i++) {
        float sensorArray[5] = {0};
        sensorArray[0] = data.accel_x[i];
        // sensorArray[3] = data.pressure_hscm[i];
        // sensorArray[4] = data.pressure_mpi[i];
        // sensorArray[0] = data.accel_x[i];
        // sensorArray[1] = data.accel_y[i];
        // sensorArray[2] = data.accel_z[i];
        global_time = data.time[i];

        int state = stateLogic.determineFlightState(millis(), sensorArray);
        printf("Flight state at %lu: %lf %d\n", millis(), sensorArray[0], state);
    }

    return 0;
}
