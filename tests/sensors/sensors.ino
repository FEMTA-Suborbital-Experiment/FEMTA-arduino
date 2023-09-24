#include "SensorPoller.h"

SensorPoller sensors;

void setup() {
  Serial.begin(115200);
  sensors.init();
}

void checkAccel() {
  float accel[3];
  sensors.readAccelerometer(accel);
  Serial.print("Accel: [");
  Serial.print(accel[0]);
  Serial.print(", ");
  Serial.print(accel[1]);
  Serial.print(", ");
  Serial.print(accel[2]);
  Serial.print("], ");
}

void checkPressure() {
  float pressure[5];
  float temperature[5];
  sensors.readPressureSensors(pressure, temperature);
  Serial.print("Pressure: [");
  for (int i = 0; i < 5; i++) {
    Serial.print(pressure[i]);
    Serial.print(", ");
  }
  Serial.print("], Temperature: [");
  for (int i = 0; i < 5; i++) {
    Serial.print(temperature[i]);
    Serial.print(", ");
  }
  Serial.print("], ");
}

void checkFlowMeter() {
  float flow;
  sensors.readFlowMeter(&flow);
  Serial.print("Flow Meter: ");
  Serial.print(flow);
}

void loop() {
  // checkAccel();
  // checkPressure();
  // checkFlowMeter();
  Serial.println("Hi");
  delay(1000);
}
