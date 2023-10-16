#ifndef ATMSPHERICPROF
#define ATMSPHERICPROF
#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>


class AtomSphericProfile {
    /*
        This class stores the atomspheric data recorded during the flight experiment.
    */
    public:
        std::vector<double> pressure_mpi;
        std::vector<double> pressure_hscm;
        std::vector<double> accel_x;
        std::vector<double> accel_y;
        std::vector<double> accel_z;
        std::vector<double> time;
        size_t count = 0;
        const char* csv_file_name;

        void ParseCSV();

        AtomSphericProfile() {
            csv_file_name = "No File Inputted.";
        }

        AtomSphericProfile(const char* inputted_file_name) {
            csv_file_name = inputted_file_name;
        }
};

#endif