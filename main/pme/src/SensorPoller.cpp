#include <HSCM.h>
#include <PVC4000.h>
#include <sensirion-lf.h>
#include <MS5837.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <vector>

#include "SensorPoller.h"


SensorPoller::SensorPoller() {
    /* Assign a unique ID to this sensor at the same time */
    this->accel = Adafruit_LSM303_Accel_Unified(54321);
    if (!this->accel.begin()) {
        /* There was a problem detecting the ADXL345 ... check your connections */
        Serial.println("No LSM303 detected! Fatal error - cannot continue.");
        while (1) {}; // Hang indefinitely
    }
    if (!this->lowAltBaro.init()) {
        Serial.println("No MS5837 detected! Fatal error - cannot continue.");
        while (1) {}; // Hang indefinitely
    }
    this->lowAltBaro.setFluidDensity(1.225); // fluid density of air is
    this->accel.setRange(LSM303_RANGE_8G);
    this->accel.setMode(LSM303_MODE_NORMAL);
}

void SensorPoller::readAccelerometer(float *vec) {
    sensors_event_t event;
    this->accel.getEvent(&event);
    vec[0] = event.acceleration.x;
    vec[1] = event.acceleration.x;
    vec[2] = event.acceleration.x;
}

void SensorPoller::readLowAltBaro(float *val) {
    this->lowAltBaro.read();
    *val = this->lowAltBaro.altitude();
}

void SensorPoller::readHighAltBaro(float *val) {

}

void SensorPoller::readVector(float *vec) {
    float accel[3];
    this->readAccelerometer(accel);
    float lowAlt;
    this->readLowAltBaro(&lowAlt);
    vec[0] = accel[0];
    vec[1] = accel[1];
    vec[2] = accel[2];
    vec[3] = lowAlt;
}
