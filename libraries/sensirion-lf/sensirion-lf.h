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

#ifndef SENSIRION_LF_H
#define SENSIRION_LF_H

#include <stdint.h>

class SensirionLF
{
public:
  SensirionLF(float flowScaleFactor, float tempScaleFactor, uint8_t i2cAddress);

  int8_t init();
  int8_t readSample();

  float getFlow() const { return mFlow; }
  float getTemp() const { return mTemp; }

  bool isAirInLineDetected() const { return mAirInLineDetected; }
  bool isHighFlowDetected()  const { return mHighFlowDetected;  }

private:
  uint8_t crc8(const uint8_t* data, uint8_t len);
  int8_t  i2c_read(uint8_t addr, uint8_t* data, uint16_t count);
  int8_t  i2c_write(uint8_t addr, const uint8_t* data, uint16_t count);

  int8_t trigger_soft_reset();
  int8_t start_measurement();

  int8_t validate_crc(uint8_t* data, uint8_t word_count);

  inline float convert_and_scale(uint8_t b1, uint8_t b2, float scale_factor);

  float   mFlowScaleFactor;
  float   mTempScaleFactor;
  uint8_t mI2cAddress;

  float   mFlow;
  float   mTemp;

  bool mAirInLineDetected;
  bool mHighFlowDetected;
};

extern SensirionLF SLF3X;

// TODO: verify with LD20 hardware
// extern SensirionLF LD20;

#endif /* SENSIRION_LF_H */
