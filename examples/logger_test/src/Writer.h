#ifndef FEMTA_WRITER
#define FEMTA_WRITER

#include "AtmosphericStruct.h"
#include "Arduino.h"
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
        Writer(const char* name, const bool toBinary, const int chipSelect=4);

        int writeToFile(logType data);
        int init();
    private:
        File logFile;
        int writeToBinary(logType data);
        int writeToText(logType data);
        const char* fileName;
        bool willWriteToBinary;
        const int chipSelect;
};

#endif