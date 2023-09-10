#include "Writer.h"

Writer::Writer(const char* name, bool writeToBinary) :
    mName{name}, mWriteToBinary{writeToBinary}
{}

void Writer::writeToFile() {
    if (mWriteToBinary == true) {
        writeToBinary();
    } else {
        writeToText();
    }
}

void Writer::writeToBinary() {}

void Writer::writeToText() {}