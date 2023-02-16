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
PVC4000::PVC4000(uint8_t address=default_address)
    : m_i2cAddress {address}
{

}


void PVC4000::init()
{
    Wire.beginTransmission(m_i2cAddress);
    Wire.write();
    Wire.endTransmission();
}


float PVC4000::temperature() {
    return _temperature;
}


float PVC4000::pressure() {
    return _pressure;
}


void PVC4000::calibrate() {
    int16_t p_count = raw_lower + raw_upper;
    int16_t t_count = t_lower + t_upper;

    if (p_count <= 10000) {
        _pressure = p_count;
    } else {
        _pressure = 13.5 * (p_count - 10000) + 10000;        
    }

}


void PVC4000::read() {
    readRaw();
    calibrate();


}

/*
    Private Functions
*/
uint16_t PVC4000::checksum(uint16_t sum) {
    if (1 + !(sum) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

uint16_t PVC4000::readRaw() {
    Wire.write(Raw_Data_R);
    Wire.requestFrom(m_i2cAddress, (uint16_t) 5);
    uint16_t i, data_reg[5] = {0};
    for (i = 0; i < 5; i++) {
        delay(4);
        data_reg[i] = Wire.read();
    }
    if (checksum(data_reg[0]) != 0) {
        Serial.println("Checksum is incorrect");
        return;
    };

    raw_upper = data_reg[1];
    raw_lower = data_reg[2];
    t_upper = data_reg[3];
    t_lower = data_reg[4];
}

void PVC4000::readCalibrationTableX() {
    enterWriteMode();
    write(Cal_Tbl_X_R);
    exitWriteMode();
    Wire.requestFrom(m_i2cAddress, (uint16_t) 15);
    for (uint8_t i = 0; i < 15; i++) {
        calibrationTableX[i] = Wire.read();
    }
    Wire.endTransmission();
};

void PVC4000::readCalibrationTableY() {
    enterWriteMode();
    write(Cal_Tbl_Y_R);
    exitWriteMode();
    Wire.requestFrom(m_i2cAddress, (uint16_t) 15);
    for (uint8_t i = 0; i < 15; i++) {
        calibrationTableY[i] = Wire.read();
    }
};

void PVC4000::writeCalibrationTableX() {
    enterWriteMode();
    Wire.write(Cal_Tbl_X_W);
    for (uint8_t i = 0; i < 15; i++) {
        Wire.write(calibrationTableX[i]);
    }
    exitWriteMode();
};

void PVC4000::writeCalibrationTableY() {
    enterWriteMode();
    Wire.write(Cal_Tbl_Y_W);
    for (uint8_t i = 0; i < 15, i++;) {
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


/*
    Test
*/

int main() {
    return 0;
}