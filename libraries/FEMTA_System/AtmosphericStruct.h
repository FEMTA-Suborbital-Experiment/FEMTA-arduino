#ifndef FEMTA_ATMOSPHERIC_STRUCT
#define FEMTA_ATMOSPHERIC_STRUCT

#include "AtmosphericVector.h"

/**
 * @brief A log standard that is shared between the reader, logger, and writer
 * classes.
 * 
 */
typedef struct {
    AtmosphericVector<float> time;
    AtmosphericVector<float> lowPressure;
    AtmosphericVector<float> highPressure;
    AtmosphericVector<float> accelerationX;
    AtmosphericVector<float> accelerationY;
    AtmosphericVector<float> accelerationZ;
    
    AtmosphericVector<float> pressure0;
    AtmosphericVector<float> pressure1;
    AtmosphericVector<float> pressure2;
    AtmosphericVector<float> pressure3;
    AtmosphericVector<float> pressure4;
    
    AtmosphericVector<float> temperature0;
    AtmosphericVector<float> temperature1;
    AtmosphericVector<float> temperature2;
    AtmosphericVector<float> temperature3;
    AtmosphericVector<float> temperature4;

    AtmosphericVector<float> flowRate;
    AtmosphericVector<float> flowTemperature;
    AtmosphericVector<float> airInLine;
    AtmosphericVector<float> highFlow;
} logType;

#endif