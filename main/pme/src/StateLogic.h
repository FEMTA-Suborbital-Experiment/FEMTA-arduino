#ifndef STATELOGIC_H
#define STATELOGIC_H
//Debug include statements
#include <stdio.h>
#include <string>
#include <iostream>

//Add #ifdef statements

using namespace std;

class StateLogic
{
    public:
        int flightState;
        int prevFlightState;
        // pre-liftoff 0, liftoff pre-MECO 1, MECO pre-coast 2, coast 3, descent 4
        // string flightStates[5]; FIX THIS
        float lowVacuumPressure;
        float highVacuumPressure;
        //vector<float> accel; FIX THIS
        float timeOfLaunch;
        float timeSinceMECO;
        float timeOfMECO;
        float timeSinceLaunch;
        float timeSincePowerup;
        float accelBuffNew[15];
        float accelBuffOld[15];

        StateLogic();
        void init(float, float);

        int determineFlightState(float sensorArray[]);
};

#endif // STATELOGIC_H
