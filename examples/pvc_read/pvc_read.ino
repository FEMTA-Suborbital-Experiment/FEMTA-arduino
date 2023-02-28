#include <PVC4000.h>

PVC4000 pvc(0x50);

void setup() {
    Serial.begin(9600);
    pvc.init();
}

void loop() {
    Serial.println("Attempt read");
    int status = pvc.read();
    if (status == 0) {
        Serial.print("Pressure: ");
        Serial.print(pvc.pressure());
        Serial.println(" micron / millitorr");
        Serial.print("Temperature: ");
        Serial.println(pvc.temperature());
    } else {
        // Serial.println("Error: CRC sum did not return 1");
    }
}
