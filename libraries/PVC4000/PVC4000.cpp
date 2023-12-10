/**
 * @file PVC4000.cpp
 * 
 * @mainpage PVC4000 Pirani Vacuum MEMS Transducer
 * 
 * @section Summary
 * 
 * The PVC4000 reads and outputs the ambient pressure and temperature in 
 * the environment (assumed to be air). Unlike traditional ambient pressure
 * sensors, the readings from a PVC4000 are valid under 10,000 microns, which
 * corresponds to the threshold which vacuum pressure can be measured without the
 * effect of convection.
 * 
 * @section Utility
 * 
 * Make sure to call read() in the loop to update the pressure and temperature
 * values of the PVC. Then call pressure() and temperature() to obtain
 * a pressure and temperature value in millitorr and Celsius, respectively.
 * 
 */

#include "PVC4000.h"
#include <Wire.h>

#ifndef ARDUINO
#include <stdint.h>
#elif ARDUINO >= 100
#include <Arduino.h>
#include "Print.h"
#else
#include "WProgram.h"
#endif

/**
 * @brief The I2C address of the PVC4000 and PVC4100 models
 * 
 */
const uint16_t PVC4000::I2C_address{0x50};

/**
 * @brief Collection of read commands from PVC I2C - Application Note Page 4 
 * 
 */
const uint16_t PVC4000::Cal_Data_R{0x00}; // TODO: Find the address that corresponds to Cal_Data_R
const uint16_t PVC4000::Raw_Data_R{0xD0};
const uint16_t PVC4000::Reg1_R{0xD3};
const uint16_t PVC4000::Reg2_R{0xD4};
const uint16_t PVC4000::Cal_Tbl_X_R{0xD1};
const uint16_t PVC4000::Cal_Tbl_Y_R{0xD2};


/**
 * @brief Collection of write commands from PVC I2C - Application Note Page 4 
 * 
 */
const uint16_t PVC4000::In_W{0xF0};
const uint16_t PVC4000::Out_W{0xF1};
const uint16_t PVC4000::Reg1_W{0xE2};
const uint16_t PVC4000::Reg2_W{0xE3};
const uint16_t PVC4000::Cal_Tbl_X_W{0xE4};
const uint16_t PVC4000::Cal_Tbl_Y_W{0xE5};

/**
 * @brief Columns of the sensitivity table which maps a count to a pressure 
 * when reading above 10,000 counts
 * 
 */
const uint16_t PVC4000::baseSensitivityTableX[8] = {
    0, 144, 185, 329, 684, 1344, 2745, 5841
};
const uint32_t PVC4000::baseSensitivityTableY[8] = {
    760000, 413000, 300000, 200000, 100000, 50000, 25000, 10000
};

int16_t bigLittle(uint16_t num) {
    return (num >> 8) | (num << 8);
}


/**
 * @brief Construct a PVC4000 class using an I2C address, if necessary.
 * 
 * @param address 
 */
PVC4000::PVC4000(int address=I2C_address)
    : m_i2cAddress {address},
      firstCalibration{true},
      calibrationTableX {{0}},
      calibrationTableY {{0}}
{

}

/**
 * @brief Initialize the PVC4000.
 * 
 */
void PVC4000::init()
{
    Serial.println("Initialized PVC");
}

/**
 * @brief Obtain the temperature from the last read command.
 * 
 * @return float 
 */
float PVC4000::temperature() {
    return _temperature;
}

float PVC4000::baselineTemperature() {
    return _baselineTemperature;
}

/**
 * @brief Obtain the pressure from the last read command.
 * 
 * @return float 
 */
float PVC4000::pressure() {
    return _pressure;
}


/**
 * @brief Return a raw count corresponding to the most significant byte.
 * 
 * @return uint16_t 
 */
uint16_t PVC4000::get_raw_upper() {
    return raw_upper;
}

/**
 * @brief Return a raw count corresponding to the least significant byte.
 * 
 * @return uint16_t 
 */
uint16_t PVC4000::get_raw_lower() {
    return raw_lower;
}


/**
 * @brief Interpolate using the interpolation table and sensitivity table
 * to obtain an ambient pressure which attempts to account for convection.
 * 
 * @param x 
 * @return float 
 */
float PVC4000::interpolate(uint16_t x) {

    // https://cplusplus.com/forum/general/216928/
    int i=0;
    while (x > interpolationTableX[i]) i++;

    uint16_t xL = interpolationTableX[i];
    uint16_t xH = interpolationTableX[i+1];
    uint32_t yL = baseSensitivityTableY[i];
    uint32_t yH = baseSensitivityTableY[i+1];

    float dydx = ( yH - yL ) / ( xH - xL );

    return yL + dydx * ( x - xL );  
}


int PVC4000::getBaselineTemperature() {
    int read_size = 3;
    Wire.write(Reg2_R);
    Wire.requestFrom(m_i2cAddress, (uint16_t) read_size);
    uint16_t i, data_reg[read_size] = {0};
    for (i = 0; i < read_size; i++) {
        delay(4);
        data_reg[i] = Wire.read();
    }

    uint16_t sum = data_reg[1] + data_reg[2];
    uint16_t checksum = 1 + ~(sum);
    
    if (checksum == data_reg[0]) {
        Serial.println("Checksum is incorrect");
        return 1;
    };

    bt_lower = data_reg[1];
    bt_upper = data_reg[2];

    return 0;
}


/**
 * @brief Poll the sensor for new pressure and temperature readings.
 * 
 */
void PVC4000::calibrate() {
    uint16_t p_count = (raw_upper << 8 | raw_lower);
    uint16_t t_count = (t_upper << 8 | t_lower);
    uint16_t bt_count = (bt_upper << 8 | bt_lower);

    if (firstCalibration == true) {
        firstCalibration = false;

        readCalibrationTableX();
        readCalibrationTableY();

        coeff = (calibrationTableX[1] - calibrationTableX[0]) / 5841.00;
        interpolationTableX[0] = calibrationTableX[0];
        for (int i=1; i < 8; i++) {
            interpolationTableX[i] = interpolationTableX[0] + coeff * baseSensitivityTableX[i];
        }
    }

    // Serial.print("CalibrationX0=");
    // Serial.println(calibrationTableX[0]);
    // Serial.print("CalibrationX1=");
    // Serial.println(calibrationTableX[1]);
    // Serial.print("P count");
    // Serial.println(p_count);
 

    if (p_count <= 10000) {
        _pressure = p_count;
    } else {
        _pressure = 13.5 * (p_count - 10000) + 10000;
        // _pressure = interpolate(p_count);
    }

    // TODO: 
    _temperature = t_count / 1024.0;
    _baselineTemperature = bt_count;
}

/**
 * @brief Update the pressure and temperature values and return the status
 * based on the raw values.
 * 
 * @return int 
 */
int PVC4000::read() {
    int status = readRaw();
    if (status == 0) {
        calibrate();
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief Perform a checksum and check if the returned values are valid
 * 
 * @param checksum 
 * @param sum 
 * @return int 
 */
int PVC4000::CRC(uint16_t checksum, uint16_t sum) {
    Serial.println(checksum);
    Serial.println(sum + checksum);
    if (sum + checksum == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
 * @brief Obtain the raw pressure and temperature values.
 * 
 * @return int 
 */
int PVC4000::readRaw() {
    int read_size = 6;
    Wire.write(Raw_Data_R);
    Wire.requestFrom(m_i2cAddress, (uint16_t) read_size);
    uint16_t i, data_reg[read_size] = {0};
    for (i = 0; i < read_size; i++) {
        delay(4);
        data_reg[i] = Wire.read();
    }

    uint16_t sum = data_reg[1] + data_reg[2] + data_reg[3] + data_reg[4] + data_reg[5];
    uint16_t checksum = 1 + ~(sum);
    
    if (checksum == data_reg[0]) {
        Serial.println("Checksum is incorrect");
        return 1;
    };
    

    raw_upper = data_reg[1];
    raw_lower = data_reg[2];
    t_upper = data_reg[4];
    t_lower = data_reg[5];

    if (getBaselineTemperature() != 0) {
        return 1;
    };

    return 0;
}

/**
 * @brief Read the sensor's calibration table and store it in the class
 * X table.
 * 
 */
void PVC4000::readCalibrationTableX() {
    Wire.write(Cal_Tbl_X_R);
    Wire.requestFrom(m_i2cAddress, (uint16_t) 30);
    for (int i = 0; i < 30; i++) {
        delay(4);
        calibrationTableX[i/2] += Wire.read();
    }
};

/**
 * @brief Read the sensor's calibration table and store it in the class
 * Y table.
 * 
 */
void PVC4000::readCalibrationTableY() {
    Wire.write(Cal_Tbl_Y_R);
    Wire.requestFrom(m_i2cAddress, (uint32_t) 30);
    for (int i = 0; i < 30; i++) {
        calibrationTableY[i/2] += Wire.read();
    }
};


/**
 * @brief Write to the sensor's calibration X table using the class calibration
 * X table.
 * 
 */
void PVC4000::writeCalibrationTableX() {
    enterWriteMode();
    Wire.write(Cal_Tbl_X_W);
    for (uint8_t i = 0; i < 30; i++) {
        Wire.write(calibrationTableX[i]);
    }
    exitWriteMode();
};


/**
 * @brief Write to the sensor's calibration Y table using the class calibration
 * Y table.
 * 
 */
void PVC4000::writeCalibrationTableY() {
    enterWriteMode();
    Wire.write(Cal_Tbl_Y_W);
    for (uint8_t i = 0; i < 30, i++;) {
        Wire.write(calibrationTableY[i]);
    }
    exitWriteMode();
};

/**
 * @brief Enter write mode for the PVC.
 * 
 */
void PVC4000::enterWriteMode() {
    Wire.write(In_W);
}

/**
 * @brief Leave write mode for the PVC.
 * 
 */
void PVC4000::exitWriteMode() {
    Wire.write(Out_W);
}

/**
 * @brief Write new data to the PVC.
 * 
 */
void PVC4000::write(uint16_t data) {
    enterWriteMode();
    Wire.write(data);
    exitWriteMode();
}