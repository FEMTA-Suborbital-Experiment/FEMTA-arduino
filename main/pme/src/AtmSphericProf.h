#ifndef ATMSPHERICPROF
#define ATMSPHERICPROF
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//Make Atomospheric Profile Class
//Need class vars for pressure, acceleration, altitude and time
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