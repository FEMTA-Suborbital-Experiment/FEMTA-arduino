#include "StateLogic.h"

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