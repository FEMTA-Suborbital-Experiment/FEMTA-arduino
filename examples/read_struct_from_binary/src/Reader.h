#ifndef FEMTA_READER
#define FEMTA_READER

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "AtmosphericStruct.h"

class Reader {
    public:
        Reader();

        File readFile(const char* fileName);
};

#endif 