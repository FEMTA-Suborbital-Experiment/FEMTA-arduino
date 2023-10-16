#ifndef FEMTA_SENSOR_POLLER
#define FEMTA_SENSOR_POLLER

#include <HSCM.h>
#include <PVC4000.h>
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
        void readLowAltBaro(float *pressure, float *temp); // float, HSCM
        void readHighAltBaro(float *pressure, float *temp); // float, PVC
        void readPressureSensors(float *pressures, float *temperatures); // float[5], float[5]
        void readFlowMeter(float *flow); // float
    private:
        void initPressureSensors();

        Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
        MS5837 pressure0;
        MS5837 pressure1;
        MS5837 pressure2;
        MS5837 pressure3;
        MS5837 pressure4;

        MS5837 *pressures[5] = { &pressure0, &pressure1, &pressure2, &pressure3, &pressure4 };

        PVC4000 highAlt = PVC4000(0x50); // high altitude baro
        HSCM_PSI lowAlt = HSCM_PSI(0x20, 0, 0);

        unsigned long lastRead;
        float sensorVector[5];

        bool accelGood = true;
        bool pressuresGood[5] = {1, 1, 1, 1, 1};
        bool flowGood = true;
};

#endif
