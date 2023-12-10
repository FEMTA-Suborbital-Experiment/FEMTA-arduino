#include <HSCM.h>
#include "sensirion-lf.h"
#include <MS5837.h>
#include <PVC4000.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <vector>

#include "SensorPollerFake.h"

// int sensor_array[5][3] = {
//     {1, 0, 0},
//     {0, 1, 1},
//     {0, 0, 0},
//     {0, 0, 1},
//     {0, 1, 0},
// };

SensorPollerFake::SensorPollerFake() { }

void SensorPollerFake::init(const char* file) {
    this->data = new AtomSphericProfile(file);
    this->lastRead = 0;
}

void SensorPollerFake::readAccelerometer(float *vec) {
    vec[0] = this->data->accel_x[this->count];
    vec[1] = this->data->accel_y[this->count];
    vec[2] = this->data->accel_z[this->count];
}

void SensorPollerFake::initPressureSensors() {
    printf("init pressure sensor!");
}

void SensorPollerFake::readPressureSensors(float *pressures, float *temperatures) {
    for (int i = 0; i < 5; i++) {
        pressures[i] = 0;
        temperatures[i] = 0;
        // if (!this->pressuresGood[i]) continue;
        // digitalWrite(PIN_DPT_SELECTOR_0, sensor_array[i][2]);
        // digitalWrite(PIN_DPT_SELECTOR_1, sensor_array[i][1]);
        // digitalWrite(PIN_DPT_SELECTOR_2, sensor_array[i][0]);
        // this->pressures[i]->read();
        // pressures[i] = this->pressures[i]->pressure();
        // temperatures[i] = this->pressures[i]->temperature();
    }
}

void SensorPollerFake::readLowAltBaro(float *pressure, float *temp) {
    *pressure = this->data->pressure_hscm[this->count];
    *temp = 17;
    // int status = lowAlt.read();
    // if (status >= 2) {
    //     Serial.print("HSCM Error: ");
    //     Serial.println(status);
    //     return;
    // }
    // *pressure = lowAlt.pressure();
    // *temp = lowAlt.temperature();
}

void SensorPollerFake::readHighAltBaro(float *pressure, float *temp) {
    // *pressure = highAlt.pressure();
    // *temp = highAlt.baselineTemperature();
    *pressure = this->data->pressure_mpi[this->count];
    *temp = 17;
}

void SensorPollerFake::readFlowMeter(float *flow) {
    *flow = 0;
}

int fail_flag = 0;

int SensorPollerFake::readVector(float *vec, unsigned long time_millis) {
    // Serial.printf("%d - %d -> %d\n", time_millis, this->lastRead, 1000 / this->pollRate);
    if ((time_millis - this->lastRead) < 1000 / (this->pollRate)) return 0;
    Serial.printf("Read %d %d\n", this->count, this->len_of_array);
    this->lastRead = time_millis;

    if (this->count >= this->len_of_array) {
        Serial.printf("read chunk at %d\n", this->file_pos);
        this->count = 0;
        this->len_of_array = this->data->ParseCSVChunk(&(this->file_pos));
    }

    // Serial.println("Done reading");

    // while (this->count >= this->len_of_array || this->data->time[this->count] * 1000 < time_millis) {
    //     Serial.println("Skipahead");
    //     this->count += 1;
    //     if (this->count >= this->len_of_array) {
    //         Serial.printf("Done at %d +  %d", this->file_pos, this->count);
    //         Serial.println();
    //         while (1) {}
    //     }
    // }

    Serial.print("read at ");
    Serial.println(this->count);

    float accel[3];
    this->readAccelerometer(accel);

    float pressure[5];
    float temperature[5];
    this->readPressureSensors(pressure, temperature);

    float lowBaroP, lowBaroT;
    float hiBaroP, hiBaroT;
    this->readLowAltBaro(&lowBaroP, &lowBaroT);
    this->readHighAltBaro(&hiBaroP, &hiBaroT);

    vec[0] = accel[0];
    vec[1] = accel[1];
    vec[2] = accel[2];
    vec[3] = lowBaroP;
    vec[4] = hiBaroP;
    this->count += 1;

    return 1;
}
