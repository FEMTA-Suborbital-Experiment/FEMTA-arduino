#include "Logger.h"

Logger::Logger(int arraySize) : 
    mArraySize{arraySize}
{
    logData.time.setMaxSize(arraySize);
    logData.lowPressure.setMaxSize(arraySize);
    logData.highPressure.setMaxSize(arraySize);
    logData.acceleration.setMaxSize(arraySize);
}

int Logger::init() {
    flushArrays();
    return 0;
}

void Logger::flushArrays() {
    logData.time.clear();
    logData.lowPressure.clear();
    logData.highPressure.clear();
    logData.acceleration.clear();
}

void Logger::pushData(float t, float lp=0.0, float hp=0.0, float a=0.0) {    
    logData.time.push_back(t);
    logData.lowPressure.push_back(lp);
    logData.highPressure.push_back(hp);
    logData.acceleration.push_back(a);
}

bool Logger::isStructFilled() {
    return logData.time.isFilled();;
}