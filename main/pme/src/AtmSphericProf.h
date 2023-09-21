#ifndef ATMSPHERICPROF
#define ATMSPHERICPROF
//#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
//#include <SPI.h>
//#include <SD.h>

#define PIN_SPI_CS 4
#define COLUMN_LEN 10

class AtomSphericProfile {
    /*
        This class stores the atomspheric data recorded during the flight experiment.
    */
    public:
        float* pressure_mpi;
        float* pressure_hscm;
        float* accel_x;
        float* accel_y;
        float* accel_z;
        float* time;
        const char* csv_file_name;

        void ParseCSV();

        void PrintArray(float*, const char*);

        AtomSphericProfile() {
            csv_file_name = "No File Inputted.";
        }

        AtomSphericProfile(const char* inputted_file_name) {
            csv_file_name = inputted_file_name;
        }
};

#endif