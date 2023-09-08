#ifndef FEMTA_LOGGER
#define FEMTA_LOGGER

#include <iostream>
#include <fstream>

#include "Arduino.h"
#include "AtmosphericVector.h"

// This is configurable
#define MAX_SIZE 100

typedef struct {
    AtmosphericVector<float> time;
    AtmosphericVector<float> lowPressure;
    AtmosphericVector<float> highPressure;
    AtmosphericVector<float> acceleration;
} logType;

class Logger{
    public:
        Logger(int arraySize);
        int writeToFile();
        int init();
        void pushData(float time, float lowPressure, float highPressure, float acceleration);
        
        logType logData;
    private:
        void flushArrays();
        bool isStructFilled();

        const char* mName;    
        int mArraySize;
};

#endif