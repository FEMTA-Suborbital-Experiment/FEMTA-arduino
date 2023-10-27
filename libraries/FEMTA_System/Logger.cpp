/**
 * @file Logger.cpp
 * 
 * @mainpage FEMTA Logger
 * 
 * @section Summary
 * 
 * The FEMTA Logger takes an max capacity integer as an input for the
 * time, low pressure, high pressure, and acceleration values. Checking if
 * these structs are filled are done at the high-level, to give the user
 * control of actions to do if the structs are filled.
 * 
 * @section Logging Order
 * 
 * The FEMTA Logger takes the following order when pushing sensor values into
 * its data: time, low pressure, high pressure, and acceleration. The FEMTA
 * writer respects this order when it writes to files. 
 * 
 * @section Utility
 * 
 * Push the time, low pressure, high pressure, and acceleration values
 * read from the sensors using pushData(). These values will be stored
 * in the class struct. Check to make sure that the vector is full using
 * isStructFilled(). If it is, use the FEMTA writer class to write to a file, 
 * and flush the array.
 * 
 */

#include "Logger.h"

/**
 * @brief Construct a Logger class, which is responsible for storing
 * the time, low pressure, high pressure, and acceleraetion values.
 * 
 * @param arraySize 
 */
Logger::Logger(int arraySize) : 
    mArraySize{arraySize}
{
    logData.time.setMaxSize(arraySize);
    logData.lowPressure.setMaxSize(arraySize);
    logData.highPressure.setMaxSize(arraySize);
    logData.acceleration.setMaxSize(arraySize);
}

/**
 * @brief Initialize the Logger and flush all values.
 * 
 * @return int 
 */
int Logger::init() {
    flushArrays();
    return 0;
}

/**
 * @brief Clear the values from each buffer.
 * 
 */
void Logger::flushArrays() {
    logData.time.clear();
    logData.lowPressure.clear();
    logData.highPressure.clear();
    logData.acceleration.clear();
}

/**
 * @brief Append the time, low pressure, high pressure, and acceleration into the
 * logger's buffer.
 * 
 * @param t 
 * @param lp 
 * @param hp 
 * @param a 
 */
void Logger::pushData(float t, float lp=0.0, float hp=0.0, float a=0.0) {    
    logData.time.push_back(t);
    logData.lowPressure.push_back(lp);
    logData.highPressure.push_back(hp);
    logData.acceleration.push_back(a);
}


/**
 * @brief Check if the buffer is filled by checking the time variable, which always
 * stores data.
 * 
 * @return true 
 * @return false 
 */
bool Logger::isStructFilled() {
    return logData.time.isFilled();;
}