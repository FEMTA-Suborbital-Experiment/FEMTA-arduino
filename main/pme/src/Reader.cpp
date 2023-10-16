/**
 * @file Reader.cpp
 * @brief The FEMTA Reader takes an input binary file and outputs the contents
 * to a struct containing the time, low pressure, high pressure, and 
 * acceleration vectors.
 * 
 * @details The reader is used to save a struct into
 * a text file, for the purpose of post-processing. The reader shall
 * not be used during launch.
 * 
 * Ensure that the file used for writing exists in the microSD card. 
 * Read the file using readFile() and store the contents of the binary
 * into a logType struct using readVector().
 * 
 */

#include "Reader.h"

const int MAX_COUNTS{10};

const char* Reader::logSizeFile{"logSize"};

/**
 * @brief Construct a Reader class, which takes a chip number (usually 4)
 * for where the SD card is located.
 * 
 * @param chipSelect 
 */
Reader::Reader(int chipSelect, int bufferSize)
 : mChipSelect{chipSelect}, mBufferSize{bufferSize}
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
 * @details The writer class stores data by struct, rather than by outputting to a large
 * vector. To compensate for this, we first read each vector to a temporary vector,
 * push each index of the temporary vector to the output vector, and repeat this
 * process for each chunk of the buffer. 
 * 
 * https://stackoverflow.com/questions/31212680/c-how-to-write-several-simple-vectors-to-a-binary-file-in-one-shot
 * @return logType 
 */
logType Reader::readVector() {
    String extension(".dat");

    File logSize = SD.open(logSizeFile + extension, FILE_READ);
    logType out;
    logType holder;
    int t_size;
    int lp_size;
    int hp_size;
    int a_size;

    logSize.read(reinterpret_cast<uint8_t*>(&t_size), sizeof(t_size));

    logSize.close();
    
    out.time.resize(t_size);
    out.lowPressure.resize(t_size); 
    out.highPressure.resize(t_size); 
    out.acceleration.resize(t_size); 
    
    // TODO: Find a better implementation which directly reads the file contents to the buffer
    for (int i=0; i < t_size / mBufferSize; ++i) {
        mFile.read(reinterpret_cast<uint8_t*>(&out.time[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.lowPressure[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.highPressure[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.acceleration[i*mBufferSize]), sizeof(float)*mBufferSize);

        Serial.print("Buffer chunk index: ");
        Serial.println(i);
    }

    return out;
}