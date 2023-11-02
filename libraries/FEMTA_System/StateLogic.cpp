#include "Arduino.h"
#include "StateLogic.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//include other libraries needed

StateLogic::StateLogic() {}

static void print_floats(float* array, const char* array_name, int len_of_array);
static void print_ints(int* array, const char* array_name, int len_of_array);

void StateLogic::init_state_storage(const char* file_name, int curr_state, 
int* prev_state, float* transit_time, StateLogic::StateStorage* a_state_storage){
    this->storageFileName = file_name;
    *a_state_storage = (StateLogic::StateStorage){.current_state = curr_state, .previous_state = prev_state, .time_of_transit = transit_time};
}


void StateLogic::write_state_storage(StateLogic::StateStorage* a_storage){
  String extension(EXTENSION);
  File storageFile = SD.open(this->storageFileName + extension, O_WRITE | O_CREAT | O_TRUNC);

  if (storageFile) {
    Serial.print("Writing to file...");
    storageFile.write((const uint8_t *)(a_storage), sizeof(*a_storage)/sizeof(uint8_t));
    storageFile.close();
    Serial.println("done.");
  }
  else {
    Serial.println("Failed to write to storage file.");
  }
}


void StateLogic::read_state_storage(StateLogic::StateStorage* a_storage){
    String extension(EXTENSION);
    File readStorageFile = SD.open(this->storageFileName + extension, FILE_READ);

    if (readStorageFile) {
      Serial.print("Reading from file...");
      readStorageFile.read((uint8_t *)(a_storage), sizeof(*a_storage)/sizeof(uint8_t));
      readStorageFile.close();
      Serial.println("done.");
    } else {
      Serial.println("Error reading to file");
    }

    Serial.println("Contents of State Storage:");
    Serial.printf("Current State after reading is: %d\n", a_storage->current_state);
    print_ints(a_storage->previous_state, "previous_state", NUM_OF_TRANSITIONS);
    print_floats(a_storage->time_of_transit, "time_of_transit", NUM_OF_TRANSITIONS);
}


static void print_floats(float* array, const char* array_name, int len_of_array) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < len_of_array; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}

static void print_ints(int* array, const char* array_name, int len_of_array) {
  Serial.print(array_name);
  Serial.print(": ");
  for(int i = 0; i < len_of_array; i++) {
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
}

void StateLogic::init() {
    this->init(0, 0);
}

void StateLogic::init(float lowVacPressure, float highVacPressure) {
    flightState = 0;
    prevFlightState = 0;
    lowVacuumPressure = lowVacPressure;
    highVacuumPressure = highVacPressure;
}

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
