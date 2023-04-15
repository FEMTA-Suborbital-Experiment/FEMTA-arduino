/*
 * Copyright (c) 2019, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This is an early stage prototype; use at your own risk!
 */

// Adapted from https://github.com/Sensirion/arduino-liquid-flow-snippets/blob/master/example_00_measurement_LD20/example_00_measurement_LD20.ino

#include <Arduino.h>
#include <Wire.h>
#include "sensirion-lf.h"

static const float   SLF3X_SCALE_FACTOR_FLOW = 500.0;
static const float   SLF3X_SCALE_FACTOR_TEMP = 200.0;
static const uint8_t SLF3X_I2C_ADDRESS = 0x08;

// TODO: verify on LD20 hardware
// static const float   LD20_SCALE_FACTOR_FLOW = 1200.0;
// static const float   LD20_SCALE_FACTOR_TEMP = 200.0;
// static const uint8_t LD20_I2C_ADDRESS = 0x08;

static const uint8_t  CMD_START_MEASUREMENT_LENGTH = 2;
static const uint8_t  CMD_START_MEASUREMENT[CMD_START_MEASUREMENT_LENGTH] = { 0x36, 0x08 };
static const uint8_t  DATA_LENGTH = 9;

static const uint8_t  SOFT_RESET_I2C_ADDRESS = 0x0;
static const uint8_t  CMD_SOFT_RESET_LENGTH = 1;
static const uint8_t  CMD_SOFT_RESET[CMD_SOFT_RESET_LENGTH] = { 0x06 };
static const uint8_t  SOFT_RESET_MAX_TRIES = 10;

static const uint8_t  CHIP_RESET_DELAY = 100;
static const uint16_t CHIP_RESET_RETRY_DELAY = 500;

static const uint8_t  INITIAL_MEASURE_DELAY = 120; // LD20: 120ms; SLF3X: 50ms

SensirionLF::SensirionLF(float flowScaleFactor,
                         float tempScaleFactor,
                         uint8_t i2cAddress)
    : mFlowScaleFactor(flowScaleFactor),
      mTempScaleFactor(tempScaleFactor),
      mI2cAddress(i2cAddress),
      mAirInLineDetected(false),
      mHighFlowDetected(false)
{
}

int8_t SensirionLF::init()
{
  Wire.begin();
  if (trigger_soft_reset() != 0) {
    return 1;
  }
  return start_measurement();
}

int8_t SensirionLF::readSample()
{
  uint8_t data[DATA_LENGTH] = { 0 };

  if (i2c_read(mI2cAddress, data, DATA_LENGTH) != 0) {
    return 1;
  }
  if (validate_crc(data, 3) != 0) {
    return 2;
  }
  mFlow = convert_and_scale(data[0], data[1], mFlowScaleFactor);
  mTemp = convert_and_scale(data[3], data[4], mTempScaleFactor);
  mAirInLineDetected = data[7]       & 1U;
  mHighFlowDetected = (data[7] >> 1) & 1U;
  return 0;
}

int8_t SensirionLF::validate_crc(uint8_t* data, uint8_t word_count)
{
  // the data coming from the sensor is in the following format:
  // [MSB0][LSB0][CRC0][MSB1][LSB1][CRC1]...[MSBx][LSBx][CRCx]
  // in this function, we verify 'word_count' MSB+LSB pairs

  static const uint8_t DATA_SIZE = 2; //
  static const uint8_t STEP_SIZE = DATA_SIZE + 1; // 2 data bytes + crc

  for (int i = 0; i < word_count; ++i) {
    uint8_t pos = i * STEP_SIZE;
    if (crc8(data + pos, DATA_SIZE) != data[pos + DATA_SIZE]) {
      return 1;
    }
  }
  return 0;
}

int8_t SensirionLF::start_measurement()
{
  if (i2c_write(mI2cAddress, CMD_START_MEASUREMENT, CMD_START_MEASUREMENT_LENGTH) != 0) {
    return 1;
  }

  delay(INITIAL_MEASURE_DELAY); // Theoretically, the first measurement is available after 12ms
  return 0;
}

int8_t SensirionLF::trigger_soft_reset()
{
  uint8_t count = 0;
  while (i2c_write(SOFT_RESET_I2C_ADDRESS, CMD_SOFT_RESET, CMD_SOFT_RESET_LENGTH) != 0) {
    // Serial.println("Error while sending soft reset command, retrying...");
    delay(CHIP_RESET_RETRY_DELAY);
    ++count;
    if (count > SOFT_RESET_MAX_TRIES) {
      return 1;
    }
  }

  delay(CHIP_RESET_DELAY); // wait long enough for chip reset to complete
  return 0;
}

inline float SensirionLF::convert_and_scale(uint8_t b1, uint8_t b2, float scale_factor)
{
  // the data read from I2C for flow and temperature is a signed 16 bit
  // value, split into two bytes b1 and b2 (MSB and LSB). To obtain
  // actual flow and temperature data, we first merge the two bytes, then
  // convert to a signed int16_t, and finally scale by scale_factor that's
  // provided in the datasheet

  return (float)((int16_t)((b1 << 8) | b2)) / scale_factor;
}

uint8_t SensirionLF::crc8(const uint8_t* data, uint8_t len)
{
  // adapted from SHT21 sample code from http://www.sensirion.com/en/products/humidity-temperature/download-center/
  uint8_t crc = 0xff;
  uint8_t byteCtr;
  for (byteCtr = 0; byteCtr < len; ++byteCtr) {
    crc ^= (data[byteCtr]);
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

int8_t SensirionLF::i2c_read(uint8_t addr, uint8_t* data, uint16_t count)
{
    Wire.requestFrom(addr, count);
    if (Wire.available() != count) {
        return -1;
    }
    for (int i = 0; i < count; ++i) {
        data[i] = Wire.read();
    }
    return 0;
}

int8_t SensirionLF::i2c_write(uint8_t addr, const uint8_t* data, uint16_t count)
{
    Wire.beginTransmission(addr);
    for (int i = 0; i < count; ++i) {
        if (Wire.write(data[i]) != 1) {
            return false;
        }
    }
    if (Wire.endTransmission() != 0) {
        return -1;
    }
    return 0;
}

SensirionLF SLF3X(SLF3X_SCALE_FACTOR_FLOW,
                  SLF3X_SCALE_FACTOR_TEMP,
                  SLF3X_I2C_ADDRESS);

// TODO: verify with LD20 hardware
// SensirionLF LD20(LD20_SCALE_FACTOR_FLOW, LD20_SCALE_FACTOR_TEMP, LD20_I2C_ADDRESS);
