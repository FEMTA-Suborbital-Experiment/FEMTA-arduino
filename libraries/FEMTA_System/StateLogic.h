#ifndef STATELOGIC_H
#define STATELOGIC_H
//Debug include statements
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <SD.h>

//Add #ifdef statements

using namespace std;

#define BUFF_SIZE 15
#define NUM_OF_TRANSITIONS 8
#define EXTENSION ".dat"

enum FlightState {
    FLIGHT_STATE_PRELIFTOFF = 0,
    FLIGHT_STATE_LIFTOFF = 1,
    FLIGHT_STATE_MECO = 2,
    FLIGHT_STATE_EXP_STARTED = 3,
    FLIGHT_STATE_EXP_DONE = 4,
    FLIGHT_STATE_DESCENDING = 5,
    FLIGHT_STATE_LANDED = 6,
};

class StateLogic
{
    public:
        int flightState;
        int prevFlightState;
        const char* storageFileName;
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

        bool spikeFlag = false; // Used to detect acceleration spikes for landing

        // Ring buffers for new (t-0 to t-15) and old (t-15 to t-30) moving AccelAverages
        // For acceleration
        // TODO: Add support for pressure sensor buffering?
        float newAccelBuffer[BUFF_SIZE] = {0};
        float oldAccelBuffer[BUFF_SIZE] = {0};
        int newPtr = 0;
        int oldPtr = 0;
        float lowPBuffer[BUFF_SIZE] = {0};
        float hiPBuffer[BUFF_SIZE] = {0};
        int hiPPtr = 0;
        int hiPPtr = 0;

        typedef struct {
            int current_state;
            int* previous_state;
            float* time_of_transit;
        } StateStorage;

        void init_state_storage(const char *, int, int*, float*, StateStorage*);
        void read_state_storage (StateStorage*);
        void write_state_storage (StateStorage*);

        StateLogic();
        void init();
        void init(float, float);

        int determineFlightState(unsigned long time_millis, float sensorArray[]);
};

#endif // STATELOGIC_H
