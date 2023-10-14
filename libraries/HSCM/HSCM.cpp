/**
 * @file HSCM.cpp
 *
 * @mainpage HSCM Ambient Pressure Sensor 
 * 
 * @section Summary
 * 
 * The HSCM Ambient Pressure sensor obtains a reading of the pressure
 * and temperature in its environment (assumed to be in air). The barometer
 * is valid from 0 psi to a maximum of 100 psi (depending on the type used).
 * However, the precision at vacuum is questionable and thus not used for
 * flight state detection in the higher altitudes (>150,000 ft).
 * 
 * @section Utility
 * 
 * Make sure to include the read() function in the loop(). Then 
 * call the temperature() and pressure() to obtain the temperature 
 * and pressure in C and psi, respectively. 
 * 
 */

#include "HSCM.h"
#include <Wire.h>
#include <limits.h>

/**
 * @brief Transfer function range, which determines the maximum and 
 * minimum output counts for interpolation
 * 
 */
const float HSCM_PSI::tf_range[4][2] {
    {0.1, 0.9}, 
    {0.05, 0.95}, 
    {0.05, 0.85}, 
    {0.04, 0.94}
};

/**
 * @brief Pressure range, which is used for interpolating between the
 * maximum and minimum calibrated ranges of the pressure per the type
 * of the HSCM.
 * 
 */
const float HSCM_PSI::pressure_range[4][2] {
    {0.0, 15.0}, 
    {0.0, 30.0}, 
    {0.0, 60.0}, 
    {0.0, 100.0}
};

/**
 * @brief Construct the HSCM using the I2C address
 * 
 * @param i2cAddress 
 * @param tf_type 
 * @param p_type 
 */
HSCM_PSI::HSCM_PSI(int i2cAddress, int tf_type=0, int p_type=0) 
    : m_i2cAddress{i2cAddress}, 
      m_tf_type{tf_type}, 
      m_pressure_type{p_type}
{

}

/**
 * @brief Construct the HSCM using the types listed in the datasheet
 * 
 * @param output_type 
 * @param tf_type 
 * @param p_type 
 */
HSCM_PSI::HSCM_PSI(hscm_output_type output_type=TYPE_2, int tf_type=0, int p_type=0) 
    : m_i2cAddress{output_type}, 
      m_tf_type{tf_type}, 
      m_pressure_type{p_type}
{

}

/**
 * @brief Return the ambient temperature (in C) read by the HSCM
 * 
 * @return float 
 */
float HSCM_PSI::temperature() {
    return _temperature;
}

/**
 * @brief Return the ambient pressure (in psi) read by the HSCM
 * 
 * @return float 
 */
float HSCM_PSI::pressure() {
    return _pressure;
}


/**
 * @brief Poll the HSCM for new pressure and temperature data and check the status
 * of the read.
 * 
 * @return uint8_t 
 */
uint8_t HSCM_PSI::read() {
    uint8_t status = readRaw();
    uint16_t counts = 2 << 13;
    const uint16_t output_max = counts * tf_range[m_tf_type][1];
    const uint16_t output_min = counts * tf_range[m_tf_type][0];
    const float pressure_max = pressure_range[m_pressure_type][1];
    const float pressure_min = pressure_range[m_pressure_type][0];

    uint16_t t_counts = (t_upper + t_lower) >> 5;
    uint16_t p_counts = p_upper + p_lower;

    _temperature = (t_counts * 0.0977) - 50;
    _pressure = (p_counts - output_min) * (pressure_max - pressure_min) / (output_max - output_min);

    // TODO: Failsafes for chip statuses 3 and 4

    return status;
}


/**
 * @brief Poll the HSCM for the raw pressure and temperature counts and check
 * the status of the read.
 * 
 * @return uint8_t 
 */
uint8_t HSCM_PSI::readRaw() {  

    // https://github.com/rodan/honeywell_hsc_ssc_i2c/blob/master/hsc_ssc_i2c.cpp
    uint8_t i, data_reg[4] = {0,0,0,0};
    Wire.requestFrom(m_i2cAddress, (uint8_t) 4);
    for (i = 0; i < 4; i++) {
        delay(4);
        data_reg[i] = Wire.read();
    }

    p_upper = (data_reg[0] & 0x3f) << 8;
    p_lower = data_reg[1];
    t_upper = data_reg[2] << 8;
    t_lower = (data_reg[3] & 0xe0);
    status = (data_reg[0] & 0xc0) >> 6;

    if (t_upper + t_lower == 65535) return 4;
    return status;
}
