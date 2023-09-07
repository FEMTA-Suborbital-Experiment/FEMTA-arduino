#ifndef FEMTA_LOGGER
#define FEMTA_LOGGER

#include <iostream>
#include <fstream>
#include "Arduino.h"

class Logger{
    public:
        Logger(int arraySize, bool writeToBinary, String name);
        struct logData;
        int writeToFile();
    private:
        void flushArrays();
        int mArraySize; 
        bool mWriteToBinary
        String name;
        ofstream logFile();
};

#endif