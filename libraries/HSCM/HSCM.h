/*

    HSCM Ambient Pressure Sensor

*/

#ifndef FEMTA_HSCM_H
#define FEMTA_HSCM_H

#define HSCM_ADDRESS 0x28

#define HSCM_MEASURE_PRESSURE    0
#define HSCM_MEASURE_TEMPERATURE 1

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

typedef struct hscm_data {
  int16_t p_upper; 
  int16_t p_lower; 
  int16_t t_upper; 
  int16_t t_lower;
} hscm_data;

class HSCM_Pressure : public Adafruit_Sensor {
    public:
        HSCM_Pressure();
        void begin();
        bool getEvent(sensors_event_t *);
        void getSensor(sensor_t *);


    private:
        int32_t _sensorID;

        hscm_data raw; // Last read accelerometer data will be available here
        float getLSB(lsm303_accel_mode_t);
        uint8_t getShift(lsm303_accel_mode_t);

        void readRawData(void);

        Adafruit_I2CDevice *i2c_dev;
};

#endif