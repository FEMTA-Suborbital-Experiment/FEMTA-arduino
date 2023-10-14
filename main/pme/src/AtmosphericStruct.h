#ifndef FEMTA_ATMOSPHERIC_STRUCT
#define FEMTA_ATMOSPHERIC_STRUCT

#include "AtmosphericVector.h"

typedef struct {
    AtmosphericVector<float> time;
    AtmosphericVector<float> lowPressure;
    AtmosphericVector<float> highPressure;
    AtmosphericVector<float> acceleration;
} logType;

#endif