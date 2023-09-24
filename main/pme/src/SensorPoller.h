#ifndef FEMTA_SENSOR_POLLER
#define FEMTA_SENSOR_POLLER

// #include <HSCM.h>
// #include <PVC4000.h>
#include <MS5837.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define PIN_DPT_SELECTOR_0 12
#define PIN_DPT_SELECTOR_1 11
#define PIN_DPT_SELECTOR_2 10

class SensorPoller {
    public:
    
        // Hertz
        int pollRate = 10;

        SensorPoller();
        void init();
        void readAccelerometer(float *vec); // float[3]
        void readLowAltBaro(float *val); // float
        void readHighAltBaro(float *val); // float
        void readPressureSensors(float *pressures, float *temperatures); // float[5], float[5]
        void readFlowMeter(float *flow); // float
    private:
        Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
        MS5837 pressure;

        unsigned long lastRead;
        float sensorVector[5];
};

#endif
