#include "AtmSphericProf.h"


void AtomSphericProfile::ParseCSV() {
  // if (!SD.begin(PIN_SPI_CS)) {
  //   Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
  //   return 1; // don't do anything more:
  // }

  // open file for reading
  String csv_str;
  char current_char;
  File csv_file = SD.open(AtomSphericProfile::csv_file_name, FILE_READ);
  File csvFile = SD.open(AtomSphericProfile::csv_file_name, FILE_READ);
  unsigned long total_chars = csvFile.size();
  if (csvFile) {
    if (csvFile.available()) {
      for(unsigned long i = 0; i <= total_chars; i++){
        current_char = csvFile.read();
        csv_str += current_char;
      }
      //csv_str = csvFile.readString();
      Serial.println(csv_str);
    }

  } else {
    Serial.print(F("SD Card: error on opening file"));
  }
  csvFile.close();
  const char* csv_string = csv_str.c_str();

  CSV_Parser parser(csv_string, "fffff");
  parser.parseLeftover();
  AtomSphericProfile::pressure_mpi = (float*)parser["Pressure Mpirani (torr)"];
  AtomSphericProfile::pressure_hscm = (float*)parser["Pressure HSCM (Pa)"];
  AtomSphericProfile::accel_z = (float*)parser["Accel (x) (m/s)"];
  AtomSphericProfile::accel_y = (float*)parser["Accel (y)"];
  AtomSphericProfile::accel_x = (float*)parser["Accel (y)"];
  AtomSphericProfile::time = (float*)parser["Time (ms)"];
}


void AtomSphericProfile::PrintArray(float* array, const char* array_name) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < COLUMN_LEN; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}
