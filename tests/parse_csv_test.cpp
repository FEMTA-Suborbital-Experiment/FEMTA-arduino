#include <iostream>
#include "../main/pme/src/AtmSphericProf.cpp"


void print_vector(const char* vector_name, std::vector <double> const &a) {
    std::cout << "The vector " << vector_name << " has elements: ";

    for(int i=0; i < a.size(); i++) {
        std::cout << a.at(i) << ' ';
    }
    fputs("\n", stdout);
}

int main() {
    const char* test_csv_file = "tests/FlightProfileEx_test.csv";
    AtomSphericProfile atmspheric_prof_test(test_csv_file);
    atmspheric_prof_test.ParseCSV();
    
    print_vector("pressure_hcsm", atmspheric_prof_test.pressure_hscm);
    std::cout << "  " << std::endl;
    print_vector("pressure_mpi", atmspheric_prof_test.pressure_mpi);
    std::cout << " " << std::endl;
    print_vector("accel_x", atmspheric_prof_test.accel_x);
    std::cout << " " << std::endl;
    print_vector("accel_y", atmspheric_prof_test.accel_y);
    std::cout << " " << std::endl;
    print_vector("accel_z", atmspheric_prof_test.accel_z);
    std::cout << " " << std::endl;
    print_vector("time", atmspheric_prof_test.time);
    return 0;
}