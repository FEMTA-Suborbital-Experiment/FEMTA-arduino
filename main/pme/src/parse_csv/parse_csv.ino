#include "../AtmSphericProf.h"
#include <CSV_Parser.h>
#include <SD.h>


AtomSphericProfile::ParseCSV() {
  // if (!SD.begin(PIN_SPI_CS)) {
  //   Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
  //   return 1; // don't do anything more:
  // }

  // open file for reading
  csvFile = SD.open(AtomSphericProfile::csv_file_name, FILE_READ);
  if (csvFile) {
    if (csvFile.available()) {
      String csv_str = csvFile.readString();
      Serial.println(csv_str);
    }

    file.close();
  } else {
    Serial.print(F("SD Card: error on opening file"));
  }

  CSVParser parser(csv_str, "fffff");
  AtomSphericProfile::pressure_mpi = (float*)parser["Pressure Mpirani (torr)"];
  AtomSphericProfile::pressure_hcsm = (float*)parser["Pressure HSCM (Pa)"];
  AtomSphericProfile::accel_z = (float*)parser["Accel (x) (m/s)"];
  AtomSphericProfile::accel_y = (float*)parser["Accel (y)"];
  AtomSphericProfile::accel_x = (float*)parser["Accel (y)"];
  AtomSphericProfile::time = (float*)parser["Time (ms)"];
}


AtomSphericProfile::PrintArray(float* array, const char* array_name) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < COLUMN_LEN; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}
