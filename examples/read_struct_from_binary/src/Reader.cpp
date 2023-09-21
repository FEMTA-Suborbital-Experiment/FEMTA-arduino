#include "Reader.h"

const int MAX_COUNTS{10};

Reader::Reader(int chipSelect)
 :mChipSelect{chipSelect}
{}

int Reader::init() {
    int count = 0;
    while (!SD.begin(mChipSelect) && count < MAX_COUNTS) {
        count++;
        Serial.print("SD Card failed or not found after ");
        Serial.print(count);
        Serial.println(" attempt(s). Retrying...");
        if (count > MAX_COUNTS) {
            Serial.print("Failed to find SD card after ");
            Serial.print(MAX_COUNTS);
            Serial.print(" tries. Please connect a valid SD Card and try again.");
            return 1;
        }
    }
    return 0;
}

int Reader::readFile(const char* fileName) {
    mFile = SD.open(fileName, std::ios_base::binary);

    return mFile.available();
}

logType Reader::readVector() {
    logType out;
    int t_size;
    int lp_size;
    int hp_size;
    int a_size;

    mFile.read((uint8_t *)&t_size, sizeof(t_size));
    mFile.read((uint8_t *)&lp_size, sizeof(lp_size));
    mFile.read((uint8_t *)&hp_size, sizeof(hp_size));
    mFile.read((uint8_t *)&a_size, sizeof(a_size));
    
    mFile.read((uint8_t *)&out.time[0], sizeof(float)*out.time.size());
    mFile.read((uint8_t *)&out.lowPressure[0], sizeof(float)*out.lowPressure.size());
    mFile.read((uint8_t *)&out.highPressure[0], sizeof(float)*out.highPressure.size());
    mFile.read((uint8_t *)&out.acceleration[0], sizeof(float)*out.acceleration.size());
    
    return out;
}