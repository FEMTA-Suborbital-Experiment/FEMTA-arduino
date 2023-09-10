#ifndef FEMTA_LOGGER
#define FEMTA_LOGGER

#include <iostream>
#include <fstream>

#include "Arduino.h"
#include "AtmosphericStruct.h"

// This is configurable
#define MAX_SIZE 100

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