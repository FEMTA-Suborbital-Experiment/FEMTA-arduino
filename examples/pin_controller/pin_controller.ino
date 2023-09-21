#include "src/PinCtrl.h"

PinCtrl PinController(A1, 5, A2, 6, A3, 9, 13);

unsigned long lastTime{0};

void setup() {
    Serial.begin(9600);
    if (PinController.init() != 0) {
        Serial.println("Something went wrong with the PinController. Exiting...");
        exit(1);
    }
    lastTime = micros();
}

/**
 * @brief The difference in execution time should be below the set
 * time delay of PinController impulse (50 ms). Typically, we achieved
 * about 2 microseconds for the Feather M0.
 * 
 */
void loop() {
    Serial.println(micros() - lastTime);
    PinController.Run();
    PinController.openFlowValve1();
    PinController.openFlowValve2();
    PinController.openVentValve();
    lastTime = micros();
}