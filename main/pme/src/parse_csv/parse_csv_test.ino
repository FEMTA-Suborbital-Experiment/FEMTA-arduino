//#include "parse_csv.ino"
#include "AtmSphericProf.h"

File csvFile;

void setup() {

  const char* csv_file_name = "test.csv";
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  csvFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (csvFile) {
    Serial.print("Writing to test.csv...");
    csvFile.println("Time (ms),Pressure HSCM (Pa),Pressure Mpirani (torr),Accel (x) (m/s),Accel (y),Accel (z)");
    csvFile.println("1.00,123.12,643.13,12.7453,323.24,2345.25");
    csvFile.println("2.00,2.134,24314.13453,422.6743,24.2,5.624");
    csvFile.println("3.00,204.5233,243.24567,1245.2677,24.2,8655.24");
    csvFile.println("4.00,23.13,13242.213,23.75632,24.223,24523.5");
    csvFile.println("5.00,45.53,23.23234,42.2635,24.2,13.4145");
    csvFile.println("6.0,13.12,56.245342,25.7467,224.25,14.525");
    csvFile.println("7.0,2.521,6.2345674,1345.264,2.40,1246.24");
    csvFile.println("8.0,3.6246,77.2367454,942.2566,24.5530,2356.25");
    csvFile.println("9.0,4.4523,224546.2352,0.2645,0.36,2562.34");
    csvFile.println("10.0,644.643,13.235662,22.2567,0.256,5.36562");

    // close the file:
    csvFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  AtomSphericProfile atmspheric_prof_test(csv_file_name);
  atmspheric_prof_test.ParseCSV();
  atmspheric_prof_test.PrintArray(atmspheric_prof_test.pressure_hscm, "pressure_hscm");
  atmspheric_prof_test.PrintArray(atmspheric_prof_test.pressure_mpi, "pressure_mpi");
  atmspheric_prof_test.PrintArray(atmspheric_prof_test.accel_x, "accel_x");
  atmspheric_prof_test.PrintArray(atmspheric_prof_test.accel_y, "accel_y");
  atmspheric_prof_test.PrintArray(atmspheric_prof_test.accel_z, "accel_z");
}

void loop() {
  // put your main code here, to run repeatedly:

}

