#include <HSCM.h>
#include "sensirion-lf.h"
#include <MS5837.h>
#include <PVC4000.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <vector>

#include "SensorPoller.h"

int sensor_array[5][3] = {
    {1, 0, 0},
    {0, 1, 1},
    {0, 1, 0},
    {0, 0, 1},
    {0, 0, 0}
};

SensorPoller::SensorPoller() { }

void SensorPoller::init() {
    /* Assign a unique ID to this sensor at the same time */
    Wire.begin();

    pinMode(PIN_DPT_SELECTOR_0, OUTPUT);
    pinMode(PIN_DPT_SELECTOR_1, OUTPUT);
    pinMode(PIN_DPT_SELECTOR_2, OUTPUT);

    if (!this->accel.begin()) {
        /* There was a problem detecting the ADXL345 ... check your connections */
        Serial.println("No LSM303 detected! Fatal error - cannot continue.");
        this->accelGood = false;
        // while (1) { delay(1000); }; // Hang indefinitely
    }
    this->initPressureSensors();
    this->accel.setRange(LSM303_RANGE_8G);
    this->accel.setMode(LSM303_MODE_NORMAL);

    this->highAlt.init();

    if (SLF3X.init() != 0) {
        Serial.println("Error during SLF3X init. Continuing in failed state.");
        delay(5000);
        this->flowGood = false;
    }
}

void SensorPoller::readAccelerometer(float *vec) {
    Serial.print("Is accel good? ");
    Serial.println(this->accelGood);
    if (!this->accelGood) return;
    sensors_event_t event;
    this->accel.getEvent(&event);
    vec[0] = event.acceleration.x;
    vec[1] = event.acceleration.y;
    vec[2] = event.acceleration.z;
}

void SensorPoller::initPressureSensors() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(PIN_DPT_SELECTOR_0, sensor_array[i][2]);
        digitalWrite(PIN_DPT_SELECTOR_1, sensor_array[i][1]);
        digitalWrite(PIN_DPT_SELECTOR_2, sensor_array[i][0]);
        Serial.print("Init pressure sensor ");
        Serial.println(i);
        delay(100);
        if (!this->pressures[i]->init()) {
            Serial.print("No MS5837 detected on:");
            Serial.print(sensor_array[i][0]);
            Serial.print(sensor_array[i][1]);
            Serial.println(sensor_array[i][2]);
            delay(5000);

            this->pressuresGood[i] = 0;
        } else {
          Serial.print("Good! For: ");
          Serial.print(sensor_array[i][0]);
          Serial.print(sensor_array[i][1]);
          Serial.println(sensor_array[i][2]);
          this->pressures[i]->setModel(MS5837::MS5837_02BA);
        }
    }
}

void SensorPoller::readPressureSensors(float *pressures, float *temperatures) {
    for (int i = 0; i < 5; i++) {
        if (!this->pressuresGood[i]) continue;
        digitalWrite(PIN_DPT_SELECTOR_0, sensor_array[i][2]);
        digitalWrite(PIN_DPT_SELECTOR_1, sensor_array[i][1]);
        digitalWrite(PIN_DPT_SELECTOR_2, sensor_array[i][0]);
        this->pressures[i]->read();
        pressures[i] = this->pressures[i]->pressure();
        temperatures[i] = this->pressures[i]->temperature();
    }
}

void SensorPoller::readLowAltBaro(float *pressure, float *temp) {
    int status = lowAlt.read();
    if (status >= 2) {
        Serial.print("HSCM Error: ");
        Serial.println(status);
        return;
    }
    *pressure = lowAlt.pressure();
    *temp = lowAlt.temperature();
}

void SensorPoller::readHighAltBaro(float *pressure, float *temp) {
    int status = highAlt.read();
    if (status != 0) {
        Serial.print("PVC4000 Error: ");
        Serial.println(status);
        return;
    }

    *pressure = highAlt.pressure();
    *temp = highAlt.baselineTemperature();
}

void SensorPoller::readFlowMeter(float *flow) {
    if (!this->flowGood) return;
    int ret = SLF3X.readSample();
    if (ret == 0) {
        *flow = SLF3X.getFlow(); 
    } else {
        Serial.print("Error reading sample from SLF3X flow sensor: ");
        Serial.println(ret);
    }
}

// void SensorPoller::readVector(float *vec, unsigned long time_millis) {
//     if ((time_millis - this->lastRead) > 1000 / (this->pollRate)) return;
//     this->lastRead = time_millis;
// 
//     float accel[3];
//     this->readAccelerometer(accel);
// 
//     float pressure[5];
//     float temperature[5];
//     this->readPressureSensors(pressure, temperature);
//     vec[0] = accel[0];
//     vec[1] = accel[1];
//     vec[2] = accel[2];
//     vec[3] = 0;
// }
