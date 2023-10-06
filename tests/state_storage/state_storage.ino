#include "../../main/pme/src/StateLogic.h"


void StateLogic::write_state_storage(StateStorage storage) {
    File state_storage = SD.open(STORAGE_FILE_NAME, FILE_WRITE);
    state_storage.write((uint8_t *)&storage, sizeof(storage));
    state_storage.close()
}


void StateLogic::read_state_storage(StateStorage storage) {
    File state_storage = SD.open(STORAGE_FILE_NAME, FILE_READ);
    state_storage.read((uint8_t *)&storage, sizeof(storage));
    state_storage.close()
}