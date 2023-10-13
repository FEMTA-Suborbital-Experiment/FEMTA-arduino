/**
 * @file Writer.h
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief The writer class used in the FEMTA Suborbital Experiment for writing 
 * a struct of sensor data into file. The writer supports two writing methods:
 * writing to a text file (for ground processing) and writing to a binary (to
 * enable better performance during the experiment).
 * 
 * The FEMTA writer makes use of direct-memory access methods for the SAMD
 * architecture (Arduino Zero, Adafruit Feather M0), enabling the CPU to perform
 * other important operations, such as flight state detection, simultaneously with
 * write operations. 
 *  
 * @date 2023-09-25
 * 
 * 
 */

#ifndef FEMTA_WRITER
#define FEMTA_WRITER

#include "AtmosphericStruct.h"
#include "Arduino.h"
#include <Adafruit_ZeroDMA.h>
#include <SPI.h>
#include <SD.h>

static const int max_timeout{10};

/**
 * @brief Separate writer class.
 * 
 * 
 */
class Writer {
    public:

        // TODO: Alternate constructor which shouldn't take booleans as parameters
        Writer(const char* name, const int chipSelect=4, const bool toBinary=0, const bool overwrite=0);

        int writeToFile(logType data);
        int init();
    private:
        File logFile;
        File logSize;
        int writeToBinary(logType data);
        int writeToText(logType data);
        const char* fileName;
        bool willWriteToBinary;
        bool willOverwrite;
        const int chipSelect;

        static const String logSizeFile;
};

#endif