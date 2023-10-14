/**
 * @file Writer.cpp
 * 
 * @mainpage FEMTA Writer
 * 
 * @section Summary
 * 
 * This is the FEMTA writer purposed for taking a logType struct as input from
 * the FEMTA Logger and writing its contents to a text file or binary. Data is
 * written in the order specified in the FEMTA Logger.
 *   
 * 
 */
#include "Writer.h"

const String Writer::logSizeFile{"logSize"};

/**
 * @brief Construct a Writer class, which is responsible for the taking 
 * logType data and writing it to a file for post-processing. This takes some
 * user parameters for writing to binary (false by default) or overwriting the
 * log file (for testing purposes, false by default).   
 * 
 * @param name 
 * @param chipSelect
 * @param toBinary
 * @param overwrite  
 */
Writer::Writer(const char* name, const int chipSelect, const bool toBinary, const bool overwrite) : 
    fileName{name}, willWriteToBinary{toBinary}, chipSelect{chipSelect}, willOverwrite{overwrite}
{}


/**
 * @brief Initialize the writer by making sure the SD card can be detected. 
 * If not detected after 10 tries, time out. Also check to see if the user
 * plans to overwrite the existing log file on initialization.
 * 
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

    delay(100);

    if (willOverwrite) {
        Serial.println("Will overwrite.");

        String extension("");

        if (willWriteToBinary) {
            extension = String(".dat");
        } else {
            extension = String(".txt");
        }

        if (SD.exists(fileName + extension)) {
            File fileExists = SD.open(fileName + extension, O_TRUNC);
            SD.remove(fileName + extension);
            Serial.println("Truncated log file.");
            fileExists.close();
        }
        if (SD.exists(logSizeFile + extension)) {
            File logSizeExists = SD.open(logSizeFile + extension, O_TRUNC);
            SD.remove(logSizeFile + extension);
            Serial.println("Truncated log size.");
            logSizeExists.close();
        }
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
    logFile = SD.open(fileName + extension, FILE_WRITE);
    logSize = SD.open(logSizeFile + extension, O_READ | O_WRITE | O_CREAT);

    int vector_size{0};

    Serial.print("Initialized Old size: ");
    Serial.println(vector_size);

    logSize.read(reinterpret_cast<uint8_t*>(&vector_size), sizeof(vector_size));

    logSize.close();

    Serial.print("Stored Old size: ");
    Serial.println(vector_size);

    int t_size = data.time.size() + vector_size;
    int lp_size = data.lowPressure.size() + vector_size;
    int hp_size = data.highPressure.size() + vector_size;
    int a_size = data.acceleration.size() + vector_size;

    Serial.print("Size of logSize: ");
    Serial.println(t_size);

    logSize = SD.open(logSizeFile + extension, O_READ | O_WRITE | O_CREAT);

    // TODO: Apply DMA 
    logSize.write(reinterpret_cast<const uint8_t*>(&t_size), sizeof(t_size));    

    logFile.write(reinterpret_cast<const uint8_t*>(&data.time[0]), sizeof(float)*data.time.size());    
    logFile.write(reinterpret_cast<const uint8_t*>(&data.lowPressure[0]), sizeof(float)*data.lowPressure.size());    
    logFile.write(reinterpret_cast<const uint8_t*>(&data.highPressure[0]), sizeof(float)*data.highPressure.size());
    logFile.write(reinterpret_cast<const uint8_t*>(&data.acceleration[0]), sizeof(float)*data.acceleration.size());
    
    logFile.close();
    logSize.close();
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