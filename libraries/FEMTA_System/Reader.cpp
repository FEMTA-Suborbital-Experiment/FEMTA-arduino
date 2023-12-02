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
    int vector_size;

    logSize.read(reinterpret_cast<uint8_t*>(&vector_size), sizeof(vector_size));

    logSize.close();
    
    out.time.resize(vector_size);
    out.lowPressure.resize(vector_size); 
    out.highPressure.resize(vector_size); 
    out.accelerationX.resize(vector_size); 
    out.accelerationY.resize(vector_size); 
    out.accelerationZ.resize(vector_size); 

    out.pressure0.resize(vector_size);
    out.pressure1.resize(vector_size);
    out.pressure2.resize(vector_size);
    out.pressure3.resize(vector_size);
    out.pressure4.resize(vector_size);

    out.temperature0.resize(vector_size);
    out.temperature1.resize(vector_size);
    out.temperature2.resize(vector_size);
    out.temperature3.resize(vector_size);
    out.temperature4.resize(vector_size);

    out.flowRate.resize(vector_size);
    out.flowTemperature.resize(vector_size);
    out.airInLine.resize(vector_size);
    out.highFlow.resize(vector_size);
    
    // TODO: Find a better implementation which directly reads the file contents to the buffer
    for (int i=0; i < vector_size / mBufferSize; ++i) {
        mFile.read(reinterpret_cast<uint8_t*>(&out.time[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.lowPressure[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.highPressure[i*mBufferSize]), sizeof(float)*mBufferSize);
        
        mFile.read(reinterpret_cast<uint8_t*>(&out.accelerationX[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.accelerationY[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.accelerationZ[i*mBufferSize]), sizeof(float)*mBufferSize);

        mFile.read(reinterpret_cast<uint8_t*>(&out.pressure0[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.pressure1[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.pressure2[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.pressure3[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.pressure4[i*mBufferSize]), sizeof(float)*mBufferSize);

        mFile.read(reinterpret_cast<uint8_t*>(&out.temperature0[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.temperature1[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.temperature2[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.temperature3[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.temperature4[i*mBufferSize]), sizeof(float)*mBufferSize);

        mFile.read(reinterpret_cast<uint8_t*>(&out.flowRate[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.flowTemperature[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.airInLine[i*mBufferSize]), sizeof(float)*mBufferSize);
        mFile.read(reinterpret_cast<uint8_t*>(&out.highFlow[i*mBufferSize]), sizeof(float)*mBufferSize);
    }

    return out;
}