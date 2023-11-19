#ifndef FEMTA_SENSOR_POLLER_FAKE
#define FEMTA_SENSOR_POLLER_FAKE

#include <HSCM.h>
#include <PVC4000.h>
#include <MS5837.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "AtmSphericProf.h"

#define PIN_DPT_SELECTOR_0 12
#define PIN_DPT_SELECTOR_1 11
#define PIN_DPT_SELECTOR_2 10

// #define USE_DELAY 0

extern int WALL_TIME_MILLIS;

class SensorPollerFake {
    public:
    
        // Hertz
        int pollRate = 10;

        SensorPollerFake();
        void init(const char *file);
        void readAccelerometer(float *vec); // float[3]
        void readLowAltBaro(float *pressure, float *temp); // float, HSCM
        void readHighAltBaro(float *pressure, float *temp); // float, PVC
        void readPressureSensors(float *pressures, float *temperatures); // float[5], float[5]
        void readFlowMeter(float *flow); // float
        int readVector(float *vec, unsigned long time_millis);
    private:
        void initPressureSensors();

        unsigned long lastRead = 0;
        float sensorVector[5];
        AtomSphericProfile *data;
        int file_pos = 0;
        int count = 0;
        int len_of_array = 0;

        bool accelGood = true;
        bool pressuresGood[5] = {1, 1, 1, 1, 1};
        bool flowGood = true;

};

#endif
