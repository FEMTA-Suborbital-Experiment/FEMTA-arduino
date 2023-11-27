/**
 * @file StateLogic.cpp
 * @brief There are there main parts of StateLogic, setting start time and measuring elapsed time
 * using the RTC and reinitializing it if power is lost, logging the current, and previous flight states
 * and the time of the transition for security, in case the power is lost, and determining and changing the
 * current and previous flight states if a transition occurs.
 * 
 * 
 * @date 2023-11-26
 * 
 * 
 */

#include "Arduino.h"
#include "StateLogic.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

StateLogic::StateLogic() {}

//Used to test state storage code
static void print_floats(float* array, const char* array_name, int len_of_array);
static void print_ints(int* array, const char* array_name, int len_of_array);


/**
 * @brief This function is to be called at whatever time is determined to be the start of the experiment so that 
 * this value can be stored in SD card and the rtc can begin measuring the elapsed time
 * 
 * @param starttime_filename is the name of the file on the SD card to which the start time should be written
 * 
 * @return true if the start time of the experiment has been succesfully written 
 * to the file specified on the SD card
 * 
 * @return false if the rtc hardware could not be intialized or if the file specified could not be opened on the SD card
 */
bool StateLogic::init_rtc(const char* starttime_filename) {
    if (!this->rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        return false;
    }

    this->rtc_file_name = starttime_filename;
    String extension(EXTENSION);
    File startFile = SD.open(this->rtc_file_name + extension, O_WRITE | O_CREAT | O_TRUNC);

    DateTime start = rtc.now();
    this->start_time = start.unixtime();

    if(startFile) {
        Serial.print("Writing from file...");
        startFile.write((const uint8_t *)&(this->start_time), sizeof(this->start_time));
        startFile.close();
        Serial.println("done.");
        return true;
    }
    else {
        Serial.println("Error writing to file");
        return false;
    }
}


/**
 * @brief This function is intended to be called if a power loss in the arduino is detected so that
 * the start time of experiment can be reset to its correct unix time so that the elapsed time can 
 * continue to be measured accurately
 * 
 * @return true if the original start time of the experiment is successfully read from the SD card
 * 
 * @return false if the rtc failed to be initialized or the file on the SD card failed to open
 */
bool StateLogic::reinit_rtc() {
    if (!this->rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        return false;
    }

    String extension(EXTENSION);
    File startFile = SD.open(this->rtc_file_name + extension, FILE_READ);

    if (startFile) {
      Serial.print("Reading from file...");
      startFile.read((uint8_t *)&(this->start_time), sizeof(this->start_time));
      startFile.close();
      Serial.println("done.");
      return true;
    } else {
      Serial.println("Error reading to file");
      return false;
    }
}


/**
 * @brief This function calculates and returns the elapsed time since experiment has begun
 * 
 * @return uint16_t which is the elapsed time since the experiment has started
 */
uint16_t StateLogic::elapsed_time() {
    DateTime current = rtc.now();
    uint32_t current_time = current.unixtime();
    return (uint16_t)(current_time - this->start_time);
}


/**
 * @brief This function initializes each value in the StateStorage struct and the file to which it will be written and read.
 * 
 * @param file_name is the name of the file that is going to be written and read from on the SD card
 * @param curr_state 
 * @param prev_state 
 * @param transit_time
 * @param prev_st_idx idx of the latest logged previous state in the previous_state array
 * @param time_idx idx of the latest logged transition time in the time_of_transit array
 * @param a_state_storage 
 */
void StateLogic::init_state_storage(const char* file_name, int curr_state, 
int* prev_state, float* transit_time, int prev_st_idx, int time_idx, StateLogic::StateStorage* a_state_storage){
    this->storageFileName = file_name;
    *a_state_storage = (StateLogic::StateStorage){.current_state = curr_state, .previous_state = prev_state, 
                                                  .time_of_transit = transit_time, .prev_state_idx = prev_st_idx, .transit_time_idx = time_idx};
}


/**
 * @brief 
 * 
 * @param a_storage which is the StateStorage struct, whos data is being written to the state storage file on the SD card
 * 
 * @return true, if the file is successfully opened and written to
 * 
 * @return false, if the file was unable to be opened and written to
 */
bool StateLogic::write_state_storage(StateLogic::StateStorage* a_storage){
  String extension(EXTENSION);
  File storageFile = SD.open(this->storageFileName + extension, O_WRITE | O_CREAT | O_TRUNC);

  if (storageFile) {
    Serial.print("Writing to file...");
    storageFile.write((const uint8_t *)(a_storage), sizeof(*a_storage)/sizeof(uint8_t));
    storageFile.close();
    Serial.println("done.");
    return true;
  }
  else {
    Serial.println("Failed to write to storage file.");
    return false;
  }
}

/**
 * @brief opens the file where the contents of the written StateStorage struct are 
 * and reads them into the given StateStorage struct
 * 
 * @param a_storage the struct to which the contents of the file will be read into
 * 
 * @return true, if the file is successfully and it contents are read into the given StateStorage struct
 * 
 * @return false, if the file was unable to be opened
 */
bool StateLogic::read_state_storage(StateLogic::StateStorage* a_storage){
    String extension(EXTENSION);
    File readStorageFile = SD.open(this->storageFileName + extension, FILE_READ);

    if (readStorageFile) {
      Serial.print("Reading from file...");
      readStorageFile.read((uint8_t *)(a_storage), sizeof(*a_storage)/sizeof(uint8_t));
      readStorageFile.close();
      Serial.println("done.");

      //Used for testing the read state storage function
      // Serial.println("Contents of State Storage:");
      // Serial.printf("Current State after reading is: %d\n", a_storage->current_state);
      // print_ints(a_storage->previous_state, "previous_state", NUM_OF_TRANSITIONS);
      // print_floats(a_storage->time_of_transit, "time_of_transit", NUM_OF_TRANSITIONS);
      return true;
    } else {
      Serial.println("Error reading to file");
      return false;
    }


}

/**
 * @brief Used to test the validity of the state storage code in regards to the previous state array
 * 
 * @param array 
 * @param array_name 
 * @param len_of_array 
 */
static void print_floats(float* array, const char* array_name, int len_of_array) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < len_of_array; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}


/**
 * @brief Used to test the validity of the state storage code in regards to the time of transitions array
 * 
 * @param array 
 * @param array_name 
 * @param len_of_array 
 */
static void print_ints(int* array, const char* array_name, int len_of_array) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < len_of_array; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}


/**
 * @brief Initializing the StateLogic class if no parameters are provided
 * 
 */
void StateLogic::init() {
    this->init(0, 0);
}


/**
 * @brief Initializing the StateLogic class to start at the first flight state and pressure values for upper and lower altitudes
 * 
 * @param lowVacPressure 
 * @param highVacPressure 
 */
void StateLogic::init(float lowVacPressure, float highVacPressure) {
    flightState = 0;
    prevFlightState = 0;
    lowVacuumPressure = lowVacPressure;
    highVacuumPressure = highVacPressure;
}


/**
 * @brief The core of the flight state logic that uses a moving average of the acceleration values from different accelerometers
 * and the current elapsed time of the flight to determine the current flight state of the experiment and then update 
 * the current and previous flight state variables 
 * 
 * @param time_millis 
 * @param sensorArray
 *  
 * @return int, which is the current flight state of the experiment
 */
int StateLogic::determineFlightState(unsigned long time_millis, float sensorArray[]){
    // sensorArray = [accelX, accelY, accelZ, lowPressure, highPressure]
    //Break down sensor array by column - refer to variable decs
    //Break down accel vector into x/y/z
    float tsl; //time since launch
    float accelX = sensorArray[0];
    float accelY = sensorArray[1];
    float accelZ = sensorArray[2];
    float lowPressure = sensorArray[3]; //HSCM pressure reading
    float highPressure = sensorArray[4]; //Pirani pressure reading

    // Compute magnitude of acceleration
    float accelMag =
        sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));

    float tALiftoff = 3.95; //m/s^2 // Acceleration threshold for liftoff
    float tSLiftoff = 3.12; // Standard deviation
    float tAMECO = -11.65; // Acceleration threshold for MECO
    float tSMECO = 6.48;

    int liftoffBreaker = 150000;
    int mecoDelay = 36000; // Milliseconds after MECO to wait before 0-g (start experiment)
    int flowTime = 110000 + mecoDelay; // How long experiment should last

    // setting timestamps
    this->timeSinceMECO = time_millis - this->timeOfMECO;
    // printf("Time since MECO: %f\n", this->timeSinceMECO);
    this->timeSinceLaunch = time_millis - this->timeOfLaunch;

    if (newPtr >= BUFF_SIZE) {
        // We're wrapping over, so move a value to the old buffer
        oldAccelBuffer[oldPtr % BUFF_SIZE] = newAccelBuffer[newPtr % BUFF_SIZE];
        // printf("move to old buffer: %d %d %.3f\n", newPtr, oldPtr, newAccelBuffer[newPtr % BUFF_SIZE]);
        oldPtr += 1;
    }
    newAccelBuffer[newPtr % BUFF_SIZE] = accelMag;
    newPtr += 1;

    // Compute moving AccelAverages and standard deviations
    float oldAccelAverage = 0, oldAccelStdDev = 0;
    float newAccelAverage = 0, newAccelStdDev = 0;

    for (int i = 0; i < BUFF_SIZE; i++) {
        oldAccelAverage += oldAccelBuffer[i];
        newAccelAverage += newAccelBuffer[i];
    }
    oldAccelAverage /= BUFF_SIZE;
    newAccelAverage /= BUFF_SIZE;
    for (int i = 0; i < BUFF_SIZE; i++) {
        oldAccelStdDev += oldAccelBuffer[i] - oldAccelAverage;
        newAccelStdDev += newAccelBuffer[i] - newAccelAverage;
    }

    //buffer logic, references code once buffers are populated
    // printf("Data at %ld: %.3f %.3f, %.3f %.3f\n", time_millis, newAccelAverage, oldAccelAverage, newAccelStdDev, oldAccelStdDev);
    if(time_millis > 4000){
        if (this->flightState >= FLIGHT_STATE_DESCENDING && accelMag >= 70) {
            this->spikeFlag = true;
        }

        if (this->prevFlightState == FLIGHT_STATE_PRELIFTOFF &&
            ((newAccelAverage-oldAccelAverage)) > (tALiftoff)
            /* && PRESSURE */
        ) {
            // Entered Liftoff
            this->prevFlightState = this->flightState;
            this->timeOfLaunch = time_millis;
            this->flightState = FLIGHT_STATE_LIFTOFF; // Ascent
        }
        //include timeSinceLaunch FIX THIS
        // Absolute time threshold since launch should also be considered
        else if ((this->flightState <= FLIGHT_STATE_LIFTOFF && newAccelAverage < 1)
            || this->flightState == FLIGHT_STATE_LIFTOFF && (time_millis - this->timeOfLaunch) >= liftoffBreaker
                /* && PRESSURE */
            ) {
            // Entered MECO
            this->timeOfMECO = time_millis;
            this->prevFlightState = this->flightState;
            this->flightState = FLIGHT_STATE_MECO; // MECO
        }
        else if (this->flightState == FLIGHT_STATE_MECO && this->timeSinceMECO >= mecoDelay) {
            // Entered operational state
            this->prevFlightState = this->flightState;
            this->flightState = FLIGHT_STATE_EXP_STARTED; // Start experiment / running
        }
        else if (this->flightState == FLIGHT_STATE_EXP_STARTED && this->timeSinceMECO >= flowTime ){
            // Stopped experiment after timer
            this->prevFlightState = this->flightState;
            this->flightState = FLIGHT_STATE_EXP_DONE; // Stopped experiment
        }
        else if (this->flightState >= FLIGHT_STATE_EXP_STARTED && this->flightState < FLIGHT_STATE_DESCENDING && newAccelAverage > 4.0) {
            // Started descent
            this->prevFlightState = this->flightState;
            this->flightState = FLIGHT_STATE_DESCENDING; // Started descent
        }
        else if (this->flightState >= FLIGHT_STATE_DESCENDING && this->spikeFlag && (7 <= newAccelAverage) && (newAccelAverage <= 12)) {
            // Detect landing (drop in noise)
            this->spikeFlag = false;
            this->prevFlightState = this->flightState;
            this->flightState = FLIGHT_STATE_LANDED; // Started descent
        }
    }

    return this->flightState;
}
