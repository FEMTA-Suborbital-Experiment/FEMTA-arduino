#include "HSCM.h"
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <limits.h>

/*
    Public Functions
*/
HSCM_Pressure::HSCM() {

}

bool HSCM_Pressure::getEvent() {

}

void HSCM_Pressure::getSensor() {

}

/*
    Private Functions
*/
void HSCM_Pressure::readRawData() {
  Adafruit_BusIO_Register data_reg = Adafruit_BusIO_Register(i2c_dev, HSCM_ADDRESS, 4);

  if (!data_reg.read()) { return; }
  
  raw.p_upper = data_reg[0];
  raw.p_lower = data_reg[1];
  raw.t_upper = data_reg[2];
  raw.t_lower = data_reg[3];
}

int main() {



    return 0;
}