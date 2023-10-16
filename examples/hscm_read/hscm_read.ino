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
        readPressure();
        readTemperature();
    }
    else {
        Serial.print("Error reading HSCM. Status code: ");
        Serial.println(status);
        readPressure();
        readTemperature();
    }
}

void readPressure() {
    Serial.print("Pressure: ");
    Serial.print(hscm.pressure());
    Serial.println(" psi")
}

void readTemperature() {
    Serial.print("Temperature: ");
    Serial.print(hscm.temperature());
    Serial.println(" C")
}