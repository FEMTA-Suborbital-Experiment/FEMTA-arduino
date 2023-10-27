#include "AtmSphericProf.h"
#include "StateLogic.h"

AtomSphericProfile *data;
StateLogic logic;
const char *csv_file_name = "accel.csv";
int file_pos = 0;
int stop = 0;

void setup() {
  Serial.begin(115200);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  data = new AtomSphericProfile(csv_file_name);
  logic.init();
  delay(1000);
}

void loop() {
  if (stop) return;
  int len_of_array = data->ParseCSVChunk(&file_pos);
  if (len_of_array == 0) {
    Serial.println("Stop here!");
    stop = 1;
  }
  for (int i = 0; i < len_of_array; i++) {
    float sensorArray[5] = {0};
    float time = data->time[i] * 1000;
    sensorArray[0] = data->accel_x[i];
    // sensorArray[1] = data->accel_y[i];
    // sensorArray[2] = data->accel_z[i];
    Serial.print("Flight state at");
    Serial.print(time / 1000);
    Serial.print(":");
    Serial.print(data->accel_x[i]);
    Serial.print(" ");
    Serial.println(logic.determineFlightState(time, sensorArray));
  }
}
