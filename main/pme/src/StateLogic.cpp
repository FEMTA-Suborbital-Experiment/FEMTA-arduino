#include "Arduino.h"
#include "StateLogic.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//include other libraries needed

StateLogic::StateLogic() {}

void StateLogic::init() {
    this->init(0, 0);
}

void StateLogic::init(float lowVacPressure, float highVacPressure) {
    flightState = 0;
    prevFlightState = 0;
    lowVacuumPressure = lowVacPressure;
    highVacuumPressure = highVacPressure;
}

int StateLogic::determineFlightState(float sensorArray[]){
    // sensorArray = [accelX, accelY, accelZ, lowPressure, highPressure]
    //Break down sensor array by column - refer to variable decs
    //Break down accel vector into x/y/z
    float tsl; //time since launch
    float accelX = sensorArray[0];
    float accelY = sensorArray[1];
    float accelZ = sensorArray[2];
    float lowPressure = sensorArray[3]; //HSCM pressure reading
    float highPressure = sensorArray[4]; //Pirani pressure reading

    // Compute magnitude of acceleration
    float accelMag =
        sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));

    float tALiftoff = 4.95; //m/s^2 // Acceleration threshold for liftoff
    float tSLiftoff = 3.12; // Standard deviation
    float tAMECO = -11.65; // Acceleration threshold for MECO
    float tSMECO = 6.48;

    int mecoDelay = 22000; // Milliseconds after MECO to wait before 0-g (start experiment)
    int flowTime = 110000 + mecoDelay; // How long experiment should last

    // setting timestamps
    this->timeSinceMECO = millis() - this->timeOfMECO;
    this->timeSinceLaunch = millis() - this->timeOfLaunch;

    if (newPtr >= BUFF_SIZE) {
        // We're wrapping over, so move a value to the old buffer
        oldAccelBuffer[oldPtr % BUFF_SIZE] = newAccelBuffer[newPtr % BUFF_SIZE];
        // printf("move to old buffer: %d %d %.3f\n", newPtr, oldPtr, newAccelBuffer[newPtr % BUFF_SIZE]);
        oldPtr += 1;
    }
    newAccelBuffer[newPtr % BUFF_SIZE] = accelMag;
    newPtr += 1;

    // Compute moving AccelAverages and standard deviations
    float oldAccelAverage = 0, oldAccelStdDev = 0;
    float newAccelAverage = 0, newAccelStdDev = 0;

    for (int i = 0; i < BUFF_SIZE; i++) {
        oldAccelAverage += oldAccelBuffer[i];
        newAccelAverage += newAccelBuffer[i];
    }
    oldAccelAverage /= BUFF_SIZE;
    newAccelAverage /= BUFF_SIZE;
    for (int i = 0; i < BUFF_SIZE; i++) {
        oldAccelStdDev += oldAccelBuffer[i] - oldAccelAverage;
        newAccelStdDev += newAccelBuffer[i] - newAccelAverage;
    }

    //buffer logic, references code once buffers are populated
    // printf("Data at %ld: %.3f %.3f, %.3f %.3f\n", millis(), newAccelAverage, oldAccelAverage, newAccelStdDev, oldAccelStdDev);
    if(millis() > 4000){
        if (this->prevFlightState == 0 &&
            ((newAccelStdDev-oldAccelStdDev) + (newAccelAverage-oldAccelAverage)) > (tALiftoff+tSLiftoff)
            /* && PRESSURE */
        ) {
            // Entered Liftoff
            this->prevFlightState = this->flightState;
            this->timeOfLaunch = millis();
            this->flightState = 1; // Ascent
        }
        //include timeSinceLaunch FIX THIS
        // Absolute time threshold since launch should also be considered
        else if (this->prevFlightState == 1 &&
                (newAccelAverage) < (2)
                /* && PRESSURE */
            ) {
            // Entered MECO
            this->timeOfMECO = millis();
            this->prevFlightState = this->flightState;
            this->flightState = 2; // MECO
        }
        else if (this->prevFlightState == 2 && this->timeSinceMECO >= mecoDelay) {
            // Entered operational state
            this->prevFlightState = this->flightState;
            this->flightState = 3; // Start experiment / running
        }
        else if (this->prevFlightState == 3 && this->timeSinceMECO >= flowTime ){
            // Entered descent
            this->prevFlightState = this->flightState;
            this->flightState = 4; // Stopped experiment
        }
    }

    return this->flightState;
}
