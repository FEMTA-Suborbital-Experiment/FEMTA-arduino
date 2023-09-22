#ifndef FEMTA_SENSOR_POLLER
#define FEMTA_SENSOR_POLLER

#include <HSCM.h>
#include <PVC4000.h>
#include <sensirion-lf.h>
#include <MS5837.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class SensorPoller {
    public:
    
        // Hertz
        int pollRate = 10;

        SensorPoller();
        void readVector(float *vec);
    private:
        Adafruit_LSM303_Accel_Unified accel;
        MS5837 lowAltBaro;

        float sensorVector[5];
        void readAccelerometer(float *vec); // float[3]
        void readLowAltBaro(float *val); // float
        void readHighAltBaro(float *val); // float
};

#endif
