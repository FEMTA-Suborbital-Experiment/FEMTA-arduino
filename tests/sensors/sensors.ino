#include "SensorPoller.h"

SensorPoller sensors;

void setup() {
  while (!Serial)
  ;
  Serial.begin(115200);
  Serial.println("hello");
  sensors.init();
  Serial.println("hey");
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

void checkBaros() {
  float pressure1 = 0;
  float pressure2 = 0;
  float temp1 = 0;
  float temp2 = 0;

  sensors.readLowAltBaro(&pressure1, &temp1);
  sensors.readHighAltBaro(&pressure2, &temp2);

  Serial.print("LowAlt Baro: [");
  Serial.print(pressure1);
  Serial.print(",");
  Serial.print(temp1);
  Serial.print(",");
  Serial.print("], HighAlt Baro: [");
  Serial.print(pressure2);
  Serial.print(",");
  Serial.print(temp2);
  Serial.print("], ");
}

void checkFlowMeter() {
  float flow;
  sensors.readFlowMeter(&flow);
  Serial.print("Flow Meter: ");
  Serial.print(flow);
}

void loop() {
  checkAccel();
  checkPressure();
  checkBaros();
  // checkFlowMeter();
  Serial.println("Hi");
  delay(1000);
}
