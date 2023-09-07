#include "Logger.h"
#include <limits>

Logger::Logger(int arraySize, bool writeToBinary, String name) : 
    mArraySize{arraySize}, mWriteToBinary{writeToBinary}, mName{name}, logFile(name)
{
     struct logData{
            int[mArraySize] time;
            int[mArraySize] lowPressure;
            int[mArraySize] highPressure;
            int[mArraySize] acceleration;
        };
}

void Logger::flushArrays() {

    // The lowest float means that it is flushed. Getting this value from an index means we ignore it in the log writing operation
    logData.time[mArraySize] = {std::numeric_limits<float>::min()};
    logData.lowPressure[mArraySize] = {std::numeric_limits<float>::min()};
    logData.highPressure[mArraySize] = {std::numeric_limits<float>::min()};
    logData.acceleration[mArraySize] = {std::numeric_limits<float>::min()};
}

int Logger::writeToFile() {
    if (mWriteToBinary == true) {
        printf("Binary write");
    } else {
        printf("ASCII write");
        logFile << "test";
    }
    flushArrays();
    return 0;
}