#include "AtmSphericProf.h"


void AtomSphericProfile::ParseCSV() {
    /*
        This function opens a csv file inputted in the constructor.
        It then parses it and puts all of the data in each respective column into the 
        pressure_hcsm, pressure_mpi, accel_x, accel_y, accel_z, and time data into vectors.

        Inputs:
            None

        Outputs:
            None
    */
    
    std::ifstream csv_file;
    csv_file.open(csv_file_name);
    
    if(csv_file.fail()) {
        std::cout << "File was not succesfully opened." << std::endl;
    }
    else {
        std::cout << "File was succesfully opened." << std::endl;
        std::string line_in_file = "";
        getline(csv_file, line_in_file);

        while(getline(csv_file, line_in_file)) {
            std::string temp_str;
            std::stringstream input_str(line_in_file);

            getline(input_str, temp_str, ',');
            time.push_back(atof(temp_str.c_str()));
            getline(input_str, temp_str, ',');
            pressure_hscm.push_back(atof(temp_str.c_str()));
            getline(input_str, temp_str, ',');
            pressure_mpi.push_back(atof(temp_str.c_str()));
            getline(input_str, temp_str, ',');
            accel_x.push_back(atof(temp_str.c_str()));
            getline(input_str, temp_str, ',');
            accel_y.push_back(atof(temp_str.c_str()));
            getline(input_str, temp_str, ',');
            accel_z.push_back(atof(temp_str.c_str()));

            line_in_file = "";
        }

    }
}

