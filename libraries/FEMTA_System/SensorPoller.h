#ifndef FEMTA_SENSOR_POLLER
#define FEMTA_SENSOR_POLLER

#include <HSCM.h>
#include <PVC4000.h>
#include <sensirion-lf.h>
#include <MS5837.h>
#include <LSM303DLHC.h>

class SensorPoller {
    public:
    
        // Hertz
        int pollRate = 10;

        void addToSensorVector();
        float[] readVector();
    private:
        vector<Sensor> sensorVector;
}


#endif