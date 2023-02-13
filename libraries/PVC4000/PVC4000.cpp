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

const uint8_t default_address{0x50};
const uint8_t Cal_Data_R;
const uint8_t Raw_Data_R{0xD0};
const uint8_t Reg1_R{0xD3};
const uint8_t Reg2_R{0xD4};
const uint8_t Cal_Tbl_X_R{0xD1};
const uint8_t Cal_Tbl_Y_R{0xD2};

const uint8_t In_W{0xF0};
const uint8_t Out_W{0xF1};
const uint8_t Reg1_W{0xE2};
const uint8_t Reg2_W{0xE3};
const uint8_t Cal_Tbl_X_W{0xE4};
const uint8_t Cal_Tbl_Y_W{0xE5};

/*
    Public Functions

*/
PVC4000::PVC4000(uint8_t address = default_address)
    : m_i2cAddress {address}
{

}


void PVC4000::init()
{
    Wire.beginTransmission(m_i2cAddress);
    Wire.write();
    Wire.endTransmission();
}


void PVC4000::calibrate() {

}


void PVC4000::read() {
    calibrate();

    if (checksum() != 0) {
        Serial.println("Checksum is incorrect");
        return;
    };
}

/*
    Private Functions
*/
uint16_t PVC4000::checksum(int8_t sum) {
    if (1 + !(sum) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

void PVC4000::readCalibrationTableX() {
    Wire.beginTransmission(m_i2cAddress);
    for (uint8_t i = 0; i < 15, i++) {
        calibrationTableX[i] = Wire.read(Cal_Tbl_X_W + i*2);
    }
    Wire.endTransmission();
};

void PVC4000::readCalibrationTableY() {
    Wire.beginTransmission(m_i2cAddress);
    for (uint8_t i = 0; i < 15, i++;) {
        calibrationTableY[i] = Wire.read(Cal_Tbl_Y_W + i*2);
    }
    Wire.endTransmission();
};

void PVC4000::writeCalibrationTableX() {
    Wire.beginTransmission(m_i2cAddress);
    for (uint8_t i = 0; i < 15, i++) {
        Wire.write(Cal_Tbl_X_W + i*2);
    }
};
void PVC4000::writeCalibrationTableY() {
    Wire.beginTransmission(m_i2cAddress);
    for (uint8_t i = 0; i < 15, i++;) {
        Wire.write(Cal_Tbl_Y_W + i*2);
    }
    Wire.endTransmission();
};


/*
    Test
*/

int main() {
    return 0;
}