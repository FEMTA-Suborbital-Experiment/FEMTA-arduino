#ifndef FEMTA_WRITER
#define FEMTA_WRITER

#include <Arduino.h>
#include <SD.h>

class Writer {

    public:
        Writer(const char* name, bool writeToBinary);
        void writeToFile();
    private:
        File data;
        const char* mName;
        bool mWriteToBinary;
        void writeToBinary();
        void writeToText();
};

#endif