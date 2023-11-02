//#include "../../main/pme/src/StateLogic.h"
#include "StateLogic.h"


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  StateLogic ss_test;
  int current_state = 1;
  int previous_states[NUM_OF_TRANSITIONS] = {0};
  float time_of_transition[NUM_OF_TRANSITIONS] = {0.0};
  StateLogic::StateStorage write_state_storage;
  int read_previous_states[NUM_OF_TRANSITIONS] = {-1, -1, -1, -1};
  float read_time_transits[NUM_OF_TRANSITIONS] = {-1.0, -1.0, -1.0, -1.0};
  StateLogic::StateStorage read_state_storage =  {.current_state = -1, 
                                                  .previous_state = read_previous_states, 
                                                  .time_of_transit = read_time_transits};

  // File storageFile = SD.open("test.dat", O_WRITE | O_CREAT | O_TRUNC);

  // if (storageFile) {
  //   Serial.print("Writing to file...");
  //   // storageFile.write((const uint8_t *)(a_storage), sizeof(*a_storage)/sizeof(uint8_t));
  //   storageFile.close();
  //   Serial.println("done.");
  // }
  // else {
  //   Serial.println("Failed to write to storage file.");
  // }

  ss_test.init_state_storage("test", current_state, previous_states, time_of_transition, &write_state_storage);
  // //ss_test.init_state_storage("test_state_storage", current_state, previous_states, time_of_transition, &read_state_storage);

  ss_test.write_state_storage(&write_state_storage);
  delay(1000);
  ss_test.read_state_storage(&read_state_storage);

  for(int transit = 1; transit < 4; transit++) {
    previous_states[transit] = transit;
  }
  time_of_transition[1] = 20.34921;
  time_of_transition[2] = 50.2324;
  time_of_transition[3] = 100.231;
  current_state = 4;

  write_state_storage = (StateLogic::StateStorage){.current_state = current_state, .previous_state = previous_states, .time_of_transit = time_of_transition};
  Serial.println("AFTER UPDATING STRUCT:");
  ss_test.write_state_storage(&write_state_storage);
  delay(1000);
  ss_test.read_state_storage(&read_state_storage);
}

void loop() {}