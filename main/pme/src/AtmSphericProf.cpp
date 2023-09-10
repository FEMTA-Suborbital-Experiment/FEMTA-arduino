#include <iostream>
#include <fstream>
#include <string>
#include "AtmSphericProf.h"

//Add a function to the class that parses csv files 
    // should load a csv file
    // assign each column to its corresponding class var, which should be arrays 


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
    
    // FILE* csv_file = fopen(csv_file_name, "r");
    // fseek(csv_file, 0, SEEK_END);
    // long array_size = ftell(csv_file);
    // fseek(csv_file, 0, SEEK_SET);

    std::ifstream csv_file;
    csv_file.open(csv_file_name);
    
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

