#include "Writer.h"

/**
 * @brief Construct a Writer class, which is responsible for the taking 
 * logType data and writing it to a file for post-processing. 
 * 
 * @param name 
 * @param toBinary 
 * @param chipSelect 
 */
Writer::Writer(const char* name, const bool toBinary, const int chipSelect) : 
    fileName{name}, willWriteToBinary{toBinary}, chipSelect{chipSelect}
{}


/**
 * @brief Initialize the writer by making sure the SD card can be detected. 
 * If not detected after 10 tries, time out.
 * 
 * @return int 
 */
int Writer::init() {
    int count = 0;
    while(!SD.begin(chipSelect)) {
        if (count == 0) {
            Serial.print("Couldn't find SD Card. Retrying...");
        } else if (count == max_timeout) {
            Serial.print("\nFailed to find SD card after ");
            Serial.print(count);
            Serial.print("tries.");
            return 1;
        } else {
            Serial.print(" ");
            Serial.print(count);
        }
        count++;
        delay(1000);
    }

    return 0; 
}

/**
 * @brief Writes logType data to a File through text or binary methods. If successful,
 * the method returns 0. Otherwise, it fails at other integer values.
 * 
 * @param data 
 * @return int 
 */
int Writer::writeToFile(logType data) {
    if (willWriteToBinary) {
        return writeToBinary(data);
    } else {
        return writeToText(data);
    }
}

/**
 * @brief Serializes the struct to a binary file. This requires a reader class,
 * but it is a faster method of saving data because we lack the overhead of a
 * typical character (256 values per character versus 100 values). Note that we
 * must establish a standard as to how we store our data.
 * 
 * @param data 
 * @return int 
 */
int Writer::writeToBinary(logType data) {
    String extension(".dat");
    File logFile = SD.open(fileName + extension, O_CREAT | O_APPEND | O_WRITE);

    // TODO: Apply DMA 
    logFile.write((const uint8_t*)&data.time.size(), sizeof(data.time.size()));    
    logFile.write((const uint8_t*)&data.lowPressure.size(), sizeof(data.lowPressure.size()));    
    logFile.write((const uint8_t*)&data.highPressure.size(), sizeof(data.highPressure.size()));
    logFile.write((const uint8_t*)&data.acceleration.size(), sizeof(data.acceleration.size()));

    logFile.write((const uint8_t*)&data.time[0], sizeof(float)*data.time.size());    
    logFile.write((const uint8_t*)&data.lowPressure[0], sizeof(float)*data.lowPressure.size());    
    logFile.write((const uint8_t*)&data.highPressure[0], sizeof(float)*data.highPressure.size());
    logFile.write((const uint8_t*)&data.acceleration[0], sizeof(float)*data.acceleration.size());
    
    logFile.close();
    return 0; 
}

/**
 * @brief Writes to a .txt file. This one is easier to access simply due to the
 * file format, but takes a longer time to process since we must deal with the
 * overhead of characters. Note that we must establish a standard as to how we
 * store our data.
 * 
 * @param data 
 * @return int 
 */
int Writer::writeToText(logType data) {
    String s = " ";
    String extension(".txt");
    File logFile = SD.open(fileName + extension, FILE_WRITE);
    for (int i=0; i < data.time.size(); ++i) {
        logFile.print(
            String(data.time[i]) + s 
            + String(data.lowPressure[i]) + s 
            + String(data.highPressure[i]) + s 
            + String(data.acceleration[i]) + '\n'
        );
    }

    logFile.close();
    return 0;
}