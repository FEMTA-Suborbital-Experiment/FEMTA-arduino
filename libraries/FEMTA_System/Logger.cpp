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

    logData.accelerationX.setMaxSize(arraySize);
    logData.accelerationY.setMaxSize(arraySize);
    logData.accelerationZ.setMaxSize(arraySize);

    logData.pressure0.setMaxSize(arraySize);
    logData.pressure1.setMaxSize(arraySize);
    logData.pressure2.setMaxSize(arraySize);
    logData.pressure3.setMaxSize(arraySize);
    logData.pressure4.setMaxSize(arraySize);

    logData.temperature0.setMaxSize(arraySize);
    logData.temperature1.setMaxSize(arraySize);
    logData.temperature2.setMaxSize(arraySize);
    logData.temperature3.setMaxSize(arraySize);
    logData.temperature4.setMaxSize(arraySize);
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
    
    logData.accelerationX.clear();
    logData.accelerationY.clear();
    logData.accelerationZ.clear();

    logData.pressure0.clear();
    logData.pressure1.clear();
    logData.pressure2.clear();
    logData.pressure3.clear();
    logData.pressure4.clear();

    logData.temperature0.clear();
    logData.temperature1.clear();
    logData.temperature2.clear();
    logData.temperature3.clear();
    logData.temperature4.clear();

    logData.flowRate.clear();
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
void Logger::pushData(float t, float *vec, float *pres, float *temps, float *flow) {    
    logData.time.push_back(t);
    logData.accelerationX.push_back(vec[0]);
    logData.accelerationY.push_back(vec[1]);
    logData.accelerationZ.push_back(vec[2]);
    logData.lowPressure.push_back(vec[3]);
    logData.highPressure.push_back(vec[4]);
    
    logData.pressure0.push_back(pres[0]);
    logData.pressure1.push_back(pres[1]);
    logData.pressure2.push_back(pres[2]);
    logData.pressure3.push_back(pres[3]);
    logData.pressure4.push_back(pres[4]);

    logData.temperature0.push_back(temps[0]);
    logData.temperature1.push_back(temps[1]);
    logData.temperature2.push_back(temps[2]);
    logData.temperature3.push_back(temps[3]);
    logData.temperature4.push_back(temps[4]);

    logData.flowRate.push_back(flow[0]);
    logData.flowTemperature.push_back(flow[1]);
    logData.airInLine.push_back(flow[2]);
    logData.highFlow.push_back(flow[3]);
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