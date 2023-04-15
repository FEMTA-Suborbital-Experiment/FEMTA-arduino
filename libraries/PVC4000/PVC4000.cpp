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

const uint16_t PVC4000::I2C_address{0x50};
const uint16_t PVC4000::Cal_Data_R{0x00}; // TODO: Find the address that corresponds to Cal_Data_R
const uint16_t PVC4000::Raw_Data_R{0xD0};
const uint16_t PVC4000::Reg1_R{0xD3};
const uint16_t PVC4000::Reg2_R{0xD4};
const uint16_t PVC4000::Cal_Tbl_X_R{0xD1};
const uint16_t PVC4000::Cal_Tbl_Y_R{0xD2};

const uint16_t PVC4000::In_W{0xF0};
const uint16_t PVC4000::Out_W{0xF1};
const uint16_t PVC4000::Reg1_W{0xE2};
const uint16_t PVC4000::Reg2_W{0xE3};
const uint16_t PVC4000::Cal_Tbl_X_W{0xE4};
const uint16_t PVC4000::Cal_Tbl_Y_W{0xE5};

const uint16_t PVC4000::baseSensitivityTableX[8] = {
    0, 144, 185, 329, 684, 1344, 2745, 5841
};
const uint32_t PVC4000::baseSensitivityTableY[8] = {
    760000, 413000, 300000, 200000, 100000, 50000, 25000, 10000
};

int16_t bigLittle(uint16_t num) {
    return (num >> 8) | (num << 8);
}


/*
    Public Functions

*/
PVC4000::PVC4000(int address=I2C_address)
    : m_i2cAddress {address},
      firstCalibration{true},
      calibrationTableX {{0}},
      calibrationTableY {{0}}
{

}


void PVC4000::init()
{
    Serial.println("Initialized PVC");
}


float PVC4000::temperature() {
    return _temperature;
}


float PVC4000::pressure() {
    return _pressure;
}


uint16_t PVC4000::get_raw_upper() {
    return raw_upper;
}

uint16_t PVC4000::get_raw_lower() {
    return raw_lower;
}


// https://cplusplus.com/forum/general/216928/
float PVC4000::interpolate(uint16_t x) {
    int i=0;
    while (x > interpolationTableX[i]) i++;

    uint16_t xL = interpolationTableX[i];
    uint16_t xH = interpolationTableX[i+1];
    uint32_t yL = baseSensitivityTableY[i];
    uint32_t yH = baseSensitivityTableY[i+1];

    float dydx = ( yH - yL ) / ( xH - xL );

    return yL + dydx * ( x - xL );  
}


void PVC4000::calibrate() {
    uint16_t p_count = (raw_upper << 8 | raw_lower);
    uint16_t t_count = t_lower + t_upper;

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

    Serial.print("CalibrationX0=");
    Serial.println(calibrationTableX[0]);
    Serial.print("CalibrationX1=");
    Serial.println(calibrationTableX[1]);
    Serial.print("P count");
    Serial.println(p_count);
 

    if (p_count <= 10000) {
        _pressure = p_count;
    } else {
        _pressure = 13.5 * (p_count - 10000) + 10000;
        // _pressure = interpolate(p_count);
    }
    _temperature = t_count;
}


int PVC4000::read() {
    int status = readRaw();
    if (status == 0) {
        calibrate();
        return 0;
    } else {
        return 1;
    }
}

/*
    Private Functions
*/
int PVC4000::CRC(uint16_t checksum, uint16_t sum) {
    // Serial.print("Sum: ");
    // Serial.println(sum);
    // Serial.print("Checksum: ");
    // Serial.println(checksum);
    // Serial.print("~Checksum: ");
    Serial.println(checksum);
    Serial.println(sum + checksum);
    if (sum + checksum == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

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
    // Serial.println(checksum);
    // Serial.println(data_reg[0]);
    
    if (checksum == data_reg[0]) {
        Serial.println("Checksum is incorrect");
        return 1;
    };
    

    raw_upper = data_reg[1];
    raw_lower = data_reg[2];
    t_upper = data_reg[4];
    t_lower = data_reg[5];

    return 0;
}

void PVC4000::readCalibrationTableX() {
    Wire.write(Cal_Tbl_X_R);
    Wire.requestFrom(m_i2cAddress, (uint16_t) 30);
    for (int i = 0; i < 30; i++) {
        delay(4);
        calibrationTableX[i/2] += Wire.read();
    }
};

void PVC4000::readCalibrationTableY() {
    Wire.write(Cal_Tbl_Y_R);
    Wire.requestFrom(m_i2cAddress, (uint32_t) 30);
    for (int i = 0; i < 30; i++) {
        calibrationTableY[i/2] += Wire.read();
    }
};

void PVC4000::writeCalibrationTableX() {
    enterWriteMode();
    Wire.write(Cal_Tbl_X_W);
    for (uint8_t i = 0; i < 30; i++) {
        Wire.write(calibrationTableX[i]);
    }
    exitWriteMode();
};

void PVC4000::writeCalibrationTableY() {
    enterWriteMode();
    Wire.write(Cal_Tbl_Y_W);
    for (uint8_t i = 0; i < 30, i++;) {
        Wire.write(calibrationTableY[i]);
    }
    exitWriteMode();
};

void PVC4000::enterWriteMode() {
    Wire.write(In_W);
}

void PVC4000::exitWriteMode() {
    Wire.write(Out_W);
}

void PVC4000::write(uint16_t data) {
    enterWriteMode();
    Wire.write(data);
    exitWriteMode();
}