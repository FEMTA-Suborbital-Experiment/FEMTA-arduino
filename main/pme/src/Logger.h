/**
 * @file Logger.h
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief The logger class used in the FEMTA Suborbital Experiment for logging
 * sensor data and sending it to the writer class. The purpose of the logger
 * class is to store the data until it has reached its capacity and send the
 * data to write. 
 * 
 * In addition, the logger class attempts to save its data
 * in the Arduino's EEPROM addresses. In the case of a power shortage, the
 * logger will recover its history of data, at the expense of a jump in time.  
 * 
 * 
 */

#ifndef FEMTA_LOGGER
#define FEMTA_LOGGER

#include <iostream>
#include <fstream>

#include "Arduino.h"
#include "AtmosphericStruct.h"

// This is configurable
#define MAX_SIZE 100

/**
 * @brief Logger for sensor data.
 * 
 */
class Logger{
    public:
        Logger(int arraySize);
        int init();
        void pushData(float time, float lowPressure, float highPressure, float acceleration);
        void flushArrays();
        bool isStructFilled();
    
        logType logData;
    private:
        int mArraySize;
};

#endif