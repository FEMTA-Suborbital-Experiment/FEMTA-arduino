#include <HSCM.h>

HSCM_PSI hscm(0x28, 0, 0);

void setup() {
    Serial.begin(115200);
    Wire.begin();
}

void loop() {
    delay(100);
    int status = hscm.read();
    if (status < 2) {
        Serial.print("Pressure: ");
        Serial.println(hscm.pressure());
        Serial.print("Temperature: ");
        Serial.println(hscm.temperature());
    }
    else {
        Serial.print("Error reading HSCM. Status code: ");
        Serial.println(status);
        Serial.print("Pressure: ");
        Serial.println(hscm.pressure());
        Serial.print("Temperature: ");
        Serial.println(hscm.temperature());
    }
}