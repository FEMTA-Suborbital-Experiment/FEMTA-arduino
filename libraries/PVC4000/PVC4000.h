#ifndef PVC_4000_H
#define PVC_4000_H

#ifndef ARDUINO
#include <stdint.h>
#elif ARDUINO >= 100
#include <Arduino.h>
#include "Print.h"
#else
#include "WProgram.h"
#endif


typedef enum {
  PVC_4000,
  PVC_4100,
} pvcModel;

typedef struct pvc4000_raw {
    int16_t temp_data;
    int16_t sensor_data;
} pvc4000_raw;


class PVC4000 {

    static const uint16_t I2C_address;
    static const uint16_t Cal_Data_R;
    static const uint16_t Raw_Data_R;
    static const uint16_t Reg1_R;
    static const uint16_t Reg2_R;
    static const uint16_t Cal_Tbl_X_R;
    static const uint16_t Cal_Tbl_Y_R;

    static const uint16_t In_W;
    static const uint16_t Out_W;
    static const uint16_t Reg1_W;
    static const uint16_t Reg2_W;
    static const uint16_t Cal_Tbl_X_W;
    static const uint16_t Cal_Tbl_Y_W;


    static const uint16_t model_1;
    static const uint16_t model_2;

    public:    
        uint16_t m_i2cAddress;
        PVC4000(uint16_t address);

        void read();
        void calibrate();
        void init();

        float temperature();
        float pressure();

    private:
        uint16_t checksum(uint16_t sum);

        void readCalibrationTableX();
        void readCalibrationTableY();
        void writeCalibrationTableX();
        void writeCalibrationTableY();

        uint16_t writeCalibrationTemperature();
        
        uint16_t readRaw();


        uint16_t[15] calibrationTableX;
        uint16_t[15] calibrationTableY;
        pvc4000_raw raw;

};


#endif