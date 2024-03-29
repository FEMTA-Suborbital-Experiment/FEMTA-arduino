/**
 * @file buffer_write_optimize.ino
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief Obtain write times and read times with respect to buffer size. Monitor the serial program to begin the program and obtain values
 * @details This program obtains write values with Serial. Thus, these write times do not reflect the true write times without Serial.
 * 
 * 
 * @date 2023-10-19
 * 
 */
#include <Writer.h>
#include <Reader.h>
#include <Logger.h>

// PARAMETERS

// File name for writing and reading log data
const char* logFileName{"OP01"};

// File name for saving write and reading times
const char* bufferWriteFileName{"BW"};

// Buffer size which may change the writing and reading time. 
// I've found that the maximum buffer size possible with Serial is 300
const int logBufferSize{300};

// How many times to write to file before stopping the program
const int maxWriteCount{100};

// PROGRAM

unsigned long lastTime{0};
const int chipSelect{4};

int writeCount{0};
int errorCount{0};

Logger logger(logBufferSize);
Reader reader(chipSelect, logBufferSize);
Writer writer(logFileName, chipSelect, 1, 1);

int writeTimes[maxWriteCount]{0};

void setup() {
    while (!Serial) {}

    Serial.begin(115200);

    if (logger.init() != 0) {errorCount++;}
    if (reader.init() != 0) {errorCount++;}
    if (writer.init() != 0) {errorCount++;}

    if (errorCount > 0) {
        Serial.println("Something went wrong with initializing the writer, reader, or logger classes");
        while(1);
    }
}

/**
 * @brief Push data and write until the maxWriteCount. At the max write count, we record the average write time.
 * 
 */
void loop() {
    logger.pushData(1, 1, 1, 1);

    if (logger.isStructFilled()) {
        lastTime = micros();
        writer.writeToFile(logger.logData);
        int writeTime = micros() - lastTime;

        writeTimes[writeCount] = writeTime;

        writeCount++;
    }
    if (writeCount == maxWriteCount) {
        Serial.print("Averaged write time:");
        Serial.println(mean(writeTimes, maxWriteCount));
        Serial.print("Buffer size: ");
        Serial.println(logBufferSize);
        

        if (!reader.readFile(logFileName)) {
            Serial.println("Something went wrong with reading file"); 
            while(1) {}
        };

        lastTime = micros();
        logType buf = reader.readVector();
        // Serial.print("Read time: ");
        // Serial.println(micros() - lastTime);
        
        for (int i=0; i < buf.time.size(); ++i) {
            Serial.print(buf.time[i], 2);
            Serial.print('\t');
            Serial.print(buf.lowPressure[i],2);
            Serial.print('\t');
            Serial.print(buf.highPressure[i],2);
            Serial.print('\t');
            Serial.print(buf.acceleration[i],2);
            Serial.print('\n');
        }
        
        while(1) {}
    }
}

// https://www.tutorialspoint.com/program-for-mean-and-median-of-an-unsorted-array-in-cplusplus
double mean(int arr[], int size){
   int sum = 0;
   for (int i = 0; i < size; i++)
      sum += arr[i];
   return (double)sum/(double)size;
}