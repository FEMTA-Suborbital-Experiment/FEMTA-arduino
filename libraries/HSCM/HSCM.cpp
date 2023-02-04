#include "HSCM.h"
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <limits.h>

/*
    Public Functions
*/
HSCM_Pressure::HSCM() {

}

float HSCM_Pressure::getTemperature() {

}

float HSCM_Pressure::getPressure() {

}


void HSCM_Pressure::init() {
    Wire.begin()

}


int8_t HSCM_Pressure::start_measurement() {
    if (i2c_write() != 0) {
        
    }
    
}


void HSCM_Pressure::read() {
    readRawData()

    
}


void HSCM_Pressure::convert() {
    
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