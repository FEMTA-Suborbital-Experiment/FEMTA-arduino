#include "StateLogic.h"
#include "PinCtrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//include other libraries needed

int determineFlightState(float sensorArray){
    //Break down sensor array by column - refer to variable decs
    //Break down accel vector into x/y/z
    float tsl; //time since launch
    float accelMag;
    float accelX;
    float accelY;
    float accelZ;
    float lowPressure; //HSCM pressure reading
    float highPressure; //Pirani pressure reading

    float tALiftoff = 4.95; //m/s^2
    float tSLiftoff = 3.12;
    float tAMECO = -11.65;
    float tSMECO = 6.48;

    int flightState;
    int buffSize = 15;

    float temp;
    float average;

    float aBuffNAve;
    float aBuffOAve;
    float sBuffNAve;
    float sBuffOAve;

    int mecoDelay = 22000;
    int flowTime = 130000;

    int i;
    int j;


    accelMag = sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));
    StateLogic.prevFlightState = StateLogic.flightState;

    // setting timestamps
    if(StateLogic.prevFlightState > 1){
        StateLogic.timeSinceMECO = millis() - StateLogic.timeOfMECO;
    }
    else if(StateLogic.prevFlightState > 0){
        StateLogic.timeSinceLaunch = millis() - StateLogic.timeOfLaunch;
    }
    
    //populating buffer
    //FIX BUFF NAMES
    //Need to update accelMag value on each loop (each loop needs to pull in a new value)
    // maximum i value needs to be number of datapoints for profile
    for (i = 0; i < 999; i++) {
        temp = accelMag; // Simulating getting new data
        if (i < 30) {
            for (j = i; j >= 1; j--) {
                aBuffNAve[j] = aBuffNAve[j - 1];
            }
            aBuffNAve[0] = temp;
        } else if (i < 60) {
            for (j = 29; j >= 1; j--) {
                aBuffNAve[j] = aBuffNAve[j - 1];
            }
            aBuffNAve[0] = temp;
            temp = aBuffNAve[29];
            for (j = i - 30; j >= 1; j--) {
                aBuffOAve[j] = aBuffOAve[j - 1];
            }
            aBuffOAve[0] = temp;
        } else {
                for (j = 29; j >= 1; j--) {
                    aBuffNAve[j] = aBuffNAve[j - 1]; // Slide all the buffer points to the right for the newer buffer.
                    aBuffOAve[j] = aBuffOAve[j - 1]; // Same as above but with the older buffer
                }
                aBuffNAve[0] = temp; // Set first point of new buffer to newly aquired accelerometer data
                temp = aBuffNAve[29];
                aBuffOAve[0] = temp; // Set first value of older buffer to last value of newer buffer (sliding)
        }
        if (i > 60) {
            for (j = 0; j < 30; j++) {
                average[0] += aBuffNAve[j];
                average[1] += aBuffOAve[j];
            }
            average[0] /= 30;
            average[1] /= 30;
            if ((average[0] - average[1] >= accelThresh) && lastState != 1) {
                printf("\nEntered Liftoff at index %d", i);
                if (i < 100){
                    printf(": this is a phantom detection of liftoff.");
                }
                lastState = 1;
            }
            else if ((average[1] - average[0] >= accelThresh) && lastState != 2) {
                printf("\nEntered MECO at index %d", i);
                if (i < 200){
                    printf(": this is a phantom detection of MECO");
                }
                lastState = 2;
            }

        average[1] = 0;
        average[2] = 0;
        }
    }


    //buffer logic, references code once buffers are populated
    if(millis() > 10000){

        if (StateLogic.prevFlightState == 0) && ((SBuffNAve-SBuffOAve + ABuffNAve-ABuffOAve) > (tALiftoff+tSLiftoff)) /* && PRESSURE */{
            // Entered Liftoff
            StateLogic.timeOfLaunch = millis();
            StateLogic.flightState = 1;
        }
        //include timeSinceLaunch FIX THIS
        else if (StateLogic.prevFlightState == 1) && ((SBuffNAve-SBuffOAve + ABuffNAve-ABuffOAve) > (tSMECO-tMECO)) /* && PRESSURE */{
            // Entered MECO
            StateLogic.timeOfMECO = millis();
            StateLogic.flightState = 2;
        }
        else if (StateLogic.prevFlightState == 2) && ((millis() - StateLogic.timeOfMECO) >= mecoDelay){
            // Entered operational state
            StateLogic.flightState = 3;
        }
        else if (StateLogic.prevFlightState == 3) && ((millis() - StateLogic.timeOfMECO) >= flowTime){
            // Entered descent
            StateLogic.flightState = 4;
        }
    }



}