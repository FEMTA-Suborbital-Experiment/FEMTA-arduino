#ifndef ATMSPHERICPROF
#define ATMSPHERICPROF
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SD.h>

#define PIN_SPI_CS 4
#define LEN_OF_CHUNK 50
#define BUFFER_SIZE 250

class AtomSphericProfile {
    /*
        This class stores the atomspheric data recorded during the flight experiment.
    */
    public:
        float pressure_mpi[LEN_OF_CHUNK] = {};
        float pressure_hscm[LEN_OF_CHUNK] = {};
        float accel_x[LEN_OF_CHUNK] = {};
        float accel_y[LEN_OF_CHUNK] = {};
        float accel_z[LEN_OF_CHUNK] = {};
        float time[LEN_OF_CHUNK] = {};
        const char* csv_file_name;

        int ParseCSVChunk();

        void PrintArray(float*, const char*, int);

        AtomSphericProfile() {
            csv_file_name = "No File Inputted.";
        }

        AtomSphericProfile(const char* inputted_file_name) {
            csv_file_name = inputted_file_name;
        }
};

#endif