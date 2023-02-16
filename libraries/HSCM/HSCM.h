/*

    HSCM Ambient Pressure Sensor Driver
    Author: Jacob Valdez
    Email: jacob.a.valdez2002@gmail.com

*/

#ifndef HSCM_H
#define HSCM_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>


typedef enum {
  TYPE_2 = 0x28,
  TYPE_3 = 0x38,
  TYPE_4 = 0x48,
  TYPE_5 = 0x58,
  TYPE_6 = 0x68,
  TYPE_7 = 0x78,

} hscm_output_type;


class HSCM_PSI {
    public:

        hscm_output_type m_output_type;

        static const float tf_range[4][2];
        static const float pressure_range[4][2];

        uint8_t m_i2cAddress;

        int m_tf_type;
        int m_pressure_type;

        HSCM_PSI(int i2cAddress, int tf_type, int p_type);
        HSCM_PSI(hscm_output_type output_type, int tf_type, int p_type);

        uint8_t read();

        float pressure();
        float temperature();

    private:

        float _pressure{0.0};
        float _temperature{0.0};

        uint16_t p_upper; 
        uint16_t p_lower; 
        uint16_t t_upper; 
        uint16_t t_lower;

        uint16_t output_max;
        uint16_t output_min;

        uint8_t status;

        uint8_t readRaw();
};

#endif