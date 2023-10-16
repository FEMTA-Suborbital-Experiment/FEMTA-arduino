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
    AtmosphericVector<float> acceleration;
} logType;

#endif