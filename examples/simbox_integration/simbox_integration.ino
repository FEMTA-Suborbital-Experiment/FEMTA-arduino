/*
    Simbox Integration

    Author: Jacob Valdez

    Description:
    This is some bonus code to obtain values from the Simbox--the Simbox was developed by freshman in Fall 2020 and Spring 2021.
    
    Only sensor values are obtained, but not altitude, and no acceleration is given. Thus, this isn't the best data to use
    for the flight simulation.

    Hence, we are looking into existing flight data to enable this.
*/

#include <Wire.h>

/*
    Simbox I2C Addresses
    DAC0: pressure sensors and thermistor 5; size 10
    DAC1: thermistors 1-4; size 16
    
    The simbox was developed to provide atmospheric pressure and temperature values
    for the PCBv3. However, due to the switch from the Pi to the Arduino, and to
    other changes in between that period, we omit other values that are in the
    array.
*/ 
const int DAC0 = 0x28;
const int DAC1 = 0x29;

// The baudrate on the Simbox is 115200; therefore, set the baudrate to that.
const int BAUDRATE = 115200;


void setup() {
    Serial.begin(BAUDRATE);
    Serial.println("Starting")
    
    Wire.begin()
    Serial.println("Wire began")
}


void loop() {
    Wire.requestFrom(DAC0, 10);
    float* sensorsA = Wire.read();

    Wire.requestFrom(DAC1, 16);
    float* sensorsB = Wire.read();

    printSensorsFrom(sensorsA, sensorsB)

    if (~Wire.available()) {
        Serial.println("Wire disconnected. Stopping...")
        return;
    }
}


/*
    Inputs:
        x: Voltage
        Index: Calibration curve indicator

    Returns:
        Pressure in kPa
    
    Description:
        Convert voltages to kilopascals. This is based off the following Exp statements:
        [calibrate | propL, poly, V, kPa | 3.95405772, -0.01466985];

        // DMP331 10581671
        // A1
        [calibrate | propR, poly, V, kPa | 3.94825760, -1.31862661];

        // DMP331 10581672
        // A2
        [calibrate | CC, poly, V, kPa | 3.94928477, 0.04101970];

        // DMP331 11110585
        // A3
        [calibrate | HFE, poly, V, kPa | 3.9580327, -0.03188930];
*/


float convertTokPa(float x, int index) {
    switch (index) {
        case (index == 0):
            return 3.95405772*x - 0.01466985;
        
        case (index == 1):
            return 3.94825760*x - 1.31862661;

        case (index == 2):
            return 3.94928477*x + 0.04101970;
        
        case (index == 3):
            return 3.9580327*x - 0.03188930;
    }
}


// Altitudes are hard to correspond separately; thus, the Simbox will also need to send altitude data
// This is not realistic in the Simbox's philosophy of simulating sensor data, but it provides some context
// to pressure and temperature values received. 
void printSensorsFrom(float* A, float* B) {
    Serial.println("Pressure 1: " + String(convertTokPa(A[1], 0)));
    Serial.println("Pressure 2: " + String(convertTokPa(A[3], 1)));
    Serial.println("Pressure 3: " + String(convertTokPa(A[5], 2)));
    Serial.println("Pressure 4: " + String(convertTokPa(A[7], 3)));

    // Thermistors should output 0 since no calibration data was given
    Serial.println("Temperature 1: " + String(B[1]));
    Serial.println("Temperature 2: " + String(B[3]));
    Serial.println("Temperature 3: " + String(B[5]));
    Serial.println("Temperature 4: " + String(A[9]));
}
