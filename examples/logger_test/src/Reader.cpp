#include "Reader.h"

Reader::Reader() {}

File Reader::readFile(const char* fileName) {
    return SD.open(fileName, FILE_READ);
}