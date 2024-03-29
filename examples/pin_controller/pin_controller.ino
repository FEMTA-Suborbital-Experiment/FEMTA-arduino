#include "src/PinCtrl.h"

// SV1_CLOSE, SV1_OPEN, SV2_CLOSE, SV2_OPEN, LV1_CLOSE, LV2_OPEN, SIGNAL
PinCtrl PinController(A1, 5, A2, 6, A3, 9, 13);

unsigned long lastTime{0};
unsigned long lastTimeMu{0};
unsigned long timeDelay{1000};
bool isOpen{0};

void setup() {
    // while (!Serial) {}

    Serial.begin(9600);
    Serial.println("Begin setup");

    if (PinController.init() != 0) {
        Serial.println("Something went wrong with the PinController. Exiting...");
        exit(1);
    }
    Serial.println("Initialized pin controller");
    lastTime = millis();
    lastTimeMu = micros();
    Serial.println("Finish setup");
}

/**
 * @brief This function turns all valves on and off on a 1 second interval.
 * The difference in execution time should be below the set
 * time delay of PinController impulse (50 ms). Typically, we achieved
 * about 2 microseconds for the Feather M0.
 * 
 */
void loop() {
    // Serial.println("Loop");
    // Serial.print("Function loop: ");
    // Serial.println(micros() - lastTimeMu);
    // Serial.print("Inner loop: ");
    // Serial.println(millis() - lastTime);
    PinController.Run();
    if ((millis() - lastTime) > timeDelay) {
        isOpen = !isOpen;
        lastTime = millis();
        if (isOpen) {
            Serial.println("Open all valves");
            PinController.openFlowValve1();
            PinController.openFlowValve2();
            PinController.openVentValve();
        }
        else {
            Serial.println("Close all valves");
            PinController.closeFlowValve1();
            PinController.closeFlowValve2();
            PinController.closeVentValve(); 
        }
    }
    lastTimeMu = micros();
}