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


class PVC4000 {

    public:    
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
      
        uint16_t m_i2cAddress;
        PVC4000(int address);

        int read();
        void calibrate();
        void init();

        float temperature();
        float pressure();

        uint16_t get_raw_upper();
        uint16_t get_raw_lower();

    private:

        int CRC(uint16_t checksum, uint16_t sum);

        void readCalibrationTableX();
        void readCalibrationTableY();
        void writeCalibrationTableX();
        void writeCalibrationTableY();

        void enterWriteMode();
        void exitWriteMode();

        uint16_t writeCalibrationTemperature();
        
        void write(uint16_t data);

        float interpolate(uint16_t x);
        
        int readRaw();

        float _temperature;
        float _pressure;

        uint16_t t_upper;
        uint16_t raw_upper;
        uint16_t t_lower;
        uint16_t raw_lower;

        float coeff;

        bool firstCalibration;

        static const uint16_t baseSensitivityTableX[8];
        static const uint32_t baseSensitivityTableY[8];

        uint16_t interpolationTableX[8];
        uint16_t interpolationTableY[8];

        uint16_t calibrationTableX[15];
        uint32_t calibrationTableY[15];
};


#endif