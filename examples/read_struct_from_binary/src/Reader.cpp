#include "Reader.h"

const int MAX_COUNTS{10};

/**
 * @brief Construct a Reader class, which takes a chip number (usually 4)
 * for where the SD card is located.
 * 
 * @param chipSelect 
 */
Reader::Reader(int chipSelect)
 :mChipSelect{chipSelect}
{}

/**
 * @brief Initialize the SD card for reading
 * 
 * @return int 
 */
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

/**
 * @brief Read the file in the SD card and returns a check if it is available
 * 
 * @param fileName 
 * @return int 
 */
int Reader::readFile(const char* fileName) {
    mFile = SD.open(fileName, FILE_READ);

    return mFile.available();
}

/**
 * @brief Outputs the time, low pressure, high pressure, and acceleration values
 * into a single, large vector for reading. 
 * 
 * https://stackoverflow.com/questions/31212680/c-how-to-write-several-simple-vectors-to-a-binary-file-in-one-shot
 * @return logType 
 */
logType Reader::readVector() {
    logType out;
    int t_size;
    int lp_size;
    int hp_size;
    int a_size;

    mFile.read(reinterpret_cast<uint8_t*>(&t_size), sizeof(t_size));
    mFile.read(reinterpret_cast<uint8_t*>(&lp_size), sizeof(lp_size));
    mFile.read(reinterpret_cast<uint8_t*>(&hp_size), sizeof(hp_size));
    mFile.read(reinterpret_cast<uint8_t*>(&a_size), sizeof(a_size));
    out.time.resize(t_size);
    out.lowPressure.resize(lp_size); 
    out.highPressure.resize(hp_size); 
    out.acceleration.resize(a_size); 
    
    mFile.read(reinterpret_cast<uint8_t*>(&out.time[0]), sizeof(float)*out.time.size());
    mFile.read(reinterpret_cast<uint8_t*>(&out.lowPressure[0]), sizeof(float)*out.lowPressure.size());
    mFile.read(reinterpret_cast<uint8_t*>(&out.highPressure[0]), sizeof(float)*out.highPressure.size());
    mFile.read(reinterpret_cast<uint8_t*>(&out.acceleration[0]), sizeof(float)*out.acceleration.size());
    
    return out;
}