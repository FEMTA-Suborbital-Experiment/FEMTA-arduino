#ifndef STATELOGIC_H
#define STATELOGIC_H
//Debug include statements
#include <stdio.h>
#include <string>
#include <iostream>

//Add #ifdef statements

using namespace std;

#define BUFF_SIZE 15

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

        // Ring buffers for new (t-0 to t-15) and old (t-15 to t-30) moving AccelAverages
        // For acceleration
        // TODO: Add support for pressure sensor buffering?
        float newAccelBuffer[BUFF_SIZE] = {0};
        float oldAccelBuffer[BUFF_SIZE] = {0};
        int newPtr = 0;
        int oldPtr = 0;

        StateLogic();
        void init();
        void init(float, float);

        int determineFlightState(unsigned long time_millis, float sensorArray[]);
};

#endif // STATELOGIC_H
