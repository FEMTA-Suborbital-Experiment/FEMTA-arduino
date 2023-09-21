#include "src/PinCtrl.h"

PinCtrl PinController(A1, 5, A2, 6, A3, 9, 13);

void setup() {
    Serial.begin(9600);
    if (PinController.init() != 0) {
        Serial.println("Something went wrong with the PinController. Exiting...");
        exit(1);
    }
}

void loop() {
    delay(1000);
    PinController.closeFlowValve1();
    PinController.closeFlowValve2();
    PinController.openVentValve();
    PinController.Run();
    PinController.openFlowValve1();
    PinController.openFlowValve2();
    PinController.closeVentValve();
}