#ifndef FEMTA_READER
#define FEMTA_READER

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <iostream>

#include "AtmosphericStruct.h"

class Reader {
    public:
        Reader(int chipSelect);

        int readFile(const char* fileName);

        logType readVector();
        int init();
    private:
        int mChipSelect;
        File mFile;
};

#endif 