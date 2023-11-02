#include "AtmSphericProf.h"


int AtomSphericProfile::ParseCSVChunk(int* current_pos) {
/*
      This function opens a csv file inputted in the constructor.
      It then parses it and puts all of the data in each respective column into the 
      pressure_hcsm, pressure_mpi, accel_x, accel_y, accel_z, and time data into vectors.

      Inputs:
          None

      Outputs:
          None
*/
  String csv_str;
  char current_char;
  char* token;

  // open file for reading
  File csvFile = SD.open(this->csv_file_name, FILE_READ);
  if (!csvFile) {
    Serial.print("Failed to open: ");
    Serial.println(this->csv_file_name);
    return 0;
  }
  // Serial.println("Opened file");

  //Reading once to get past the header line of the csv file
  if (*current_pos == 0) {
    csv_str = csvFile.readStringUntil('\n');
  } else {
    csvFile.seek(*current_pos);
  }

  int end_of_file = csvFile.size();
  int linesParsed = 0;
  char buffer[BUFFER_SIZE]; // Adjust the buffer size as needed
  while (csvFile.available() && linesParsed < LEN_OF_CHUNK && *current_pos < end_of_file) {
    // Read a line into the buffer
    csv_str = csvFile.readStringUntil('\n');
    // Serial.println(csv_str);
    if(csv_str != NULL) {
      csv_str.toCharArray(buffer, sizeof(buffer));
    }
    else {
      Serial.println("Reached End of File.");
      csvFile.close();
      return linesParsed; //indicating that reached end of file
    }
    // Serial.println("Got line");

    // Tokenize the line using strtok
    token = strtok(buffer, ",");

    if (token) {
      time[linesParsed] = atof(token);
      token = strtok(NULL, ",");
      this->accel_x[linesParsed] = atof(token);
      // token = strtok(NULL, ",");
      // this->pressure_hscm[linesParsed] = atof(token);
      // token = strtok(NULL, ",");
      // this->pressure_mpi[linesParsed] = atof(token);
      // token = strtok(NULL, ",");
      // this->accel_x[linesParsed] = atof(token);
      // token = strtok(NULL, ",");
      // this->accel_y[linesParsed] = atof(token);
      // token = strtok(NULL, ",");
      // this->accel_z[linesParsed] = atof(token);

      *current_pos = csvFile.position();
      linesParsed++;
    }
  }

  // Serial.printf("Number of Lines Parsed: %d\n", linesParsed);
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
