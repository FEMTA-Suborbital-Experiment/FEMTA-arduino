#include "AtmSphericProf.h"


int AtomSphericProfile::ParseCSVChunk(int* current_pos) {
  String csv_str;
  char current_char;
  char* token;

  // open file for reading
  File csvFile = SD.open(this->csv_file_name, FILE_READ);
  if (!csvFile) {
    Serial.println("Failed to open data.csv!");
    return 0;
  }

  //Reading once to get past the header line of the csv file
  csv_str = csvFile.readStringUntil('\n');

  int end_of_file = csvFile.size();
  int linesParsed = 0;
  char buffer[BUFFER_SIZE]; // Adjust the buffer size as needed
  while (csvFile.available() && linesParsed < LEN_OF_CHUNK && *current_pos < end_of_file) {
    // Read a line into the buffer
    csv_str = csvFile.readStringUntil('\n');
    Serial.println(csv_str);
    if(csv_str != NULL) {
      csv_str.toCharArray(buffer, sizeof(buffer));
    }
    else {
      Serial.println("Reached End of File.");
      csvFile.close();
      return linesParsed; //indicating that reached end of file
    }

    // Tokenize the line using strtok
    token = strtok(buffer, ",");

    if (token) {
      time[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->pressure_hscm[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->pressure_mpi[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->accel_x[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->accel_y[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->accel_z[linesParsed] = atof(token);

      *current_pos = csvFile.position();
      linesParsed++;
    }
  }

  Serial.printf("Number of Lines Parsed: %d\n", linesParsed);
  csvFile.close();
  return linesParsed;
}


void AtomSphericProfile::PrintArray(float* array, const char* array_name, int len_of_array) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < len_of_array; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}
