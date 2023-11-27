#include <StateLogic.h>


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
  int prev_st_idx = 0;
  int time_transit_idx = 0;
  int previous_states[NUM_OF_TRANSITIONS] = {0};
  float time_of_transition[NUM_OF_TRANSITIONS] = {0.0};
  StateLogic::StateStorage write_state_storage;
  int read_previous_states[NUM_OF_TRANSITIONS] = {-1, -1, -1, -1};
  float read_time_transits[NUM_OF_TRANSITIONS] = {-1.0, -1.0, -1.0, -1.0};
  StateLogic::StateStorage read_state_storage =  {.current_state = -1, 
                                                  .previous_state = read_previous_states, 
                                                  .time_of_transit = read_time_transits,
                                                  .prev_state_idx = -1,
                                                  .transit_time_idx = -1};

  ss_test.init_state_storage("test", current_state, previous_states, time_of_transition, 
                             prev_st_idx, time_transit_idx, &write_state_storage);

  ss_test.write_state_storage(&write_state_storage);
  delay(1000);
  ss_test.read_state_storage(&read_state_storage);

  for(int transit = 1; transit < 4; transit++) {
    previous_states[transit] = transit;
    prev_st_idx += 1;
  }
  time_of_transition[1] = 20.34921;
  time_transit_idx += 1;
  time_of_transition[2] = 50.2324;
  time_transit_idx += 1;
  time_of_transition[3] = 100.231;
  time_transit_idx += 1;
  current_state = 4;


  write_state_storage = (StateLogic::StateStorage){.current_state = current_state, .previous_state = previous_states,
                                                   .time_of_transit = time_of_transition,
                                                   .prev_state_idx = prev_st_idx, .transit_time_idx = time_transit_idx};
  Serial.println("AFTER UPDATING STRUCT:");
  ss_test.write_state_storage(&write_state_storage);
  delay(1000);
  ss_test.read_state_storage(&read_state_storage);
}

void loop() {}