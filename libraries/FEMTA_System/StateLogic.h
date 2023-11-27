/**
 * @file StateLogic.cpp
 * @author Josh Mansky, Vincent Wang, Jacob Valdez (jmansky@purdue.edu, wang5528@purdue.edu ,valdez24@purdue.edu)
 * @brief The FEMTA Suborbital StateLogic Class is used to determine and store the current state of the flight.
 * 
 * @details The StateLogic class has two seperate functions, one is that it uses data inputted from the various accelerometers's
 * and pressure sensors, as well as, the current duration of the flight to determine the current state of the flight, the other 
 * function of the StateLogic class is to log the current state of the flight, the previous state, the time of the transition, and 
 * the unix time at which the experiment started. The first three values are for data analysis after the experiment, while the last
 * value as well as the current state variable are used to reset the current duration time and flight state of the experiment 
 * if a power output occurs in the arduino.
 * 
 * 
 * @date 2023-11-26
 */

#ifndef STATELOGIC_H
#define STATELOGIC_H
//Debug include statements
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <SD.h>
#include <RTClib.h>

//Add #ifdef statements

using namespace std;

#define BUFF_SIZE 15
#define NUM_OF_TRANSITIONS 8
#define EXTENSION ".dat"

/**
 * @brief Enumeration that shows all of the flight states.
 * 
 */
enum FlightState {
    FLIGHT_STATE_PRELIFTOFF = 0,
    FLIGHT_STATE_LIFTOFF = 1,
    FLIGHT_STATE_MECO = 2,
    FLIGHT_STATE_EXP_STARTED = 3,
    FLIGHT_STATE_EXP_DONE = 4,
    FLIGHT_STATE_DESCENDING = 5,
    FLIGHT_STATE_LANDED = 6,
};

/**
 * @brief The StateLogic Class.
 * 
 */
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

        /**
         * @brief A struct that holds the current, and previous flight states,
         * as well as, the time of the transition, that are written to an SD card 
         * every time the flight state changes. The struct also holds the index's
         * of the previous_state and time_of_transit arrays at which the last logged value is 
         * so that the next logged value can be logged at the index after this index.
         */
        typedef struct {
            int current_state;
            int* previous_state;
            float* time_of_transit;
            int prev_state_idx;
            int transit_time_idx;
        } StateStorage;

        
        RTC_DS3231 rtc; //Instantiating the class used to interface with the DS3231 RTC hardware
        uint32_t start_time = 0;
        const char* rtc_file_name;

        bool init_rtc(const char*);
        bool reinit_rtc();
        uint16_t elapsed_time();

        void init_state_storage(const char *, int, int*, float*, int, int, StateStorage*);
        bool read_state_storage (StateStorage*);
        bool write_state_storage (StateStorage*);

        StateLogic();
        void init();
        void init(float, float);

        int determineFlightState(unsigned long time_millis, float sensorArray[]);
};

#endif // STATELOGIC_H
