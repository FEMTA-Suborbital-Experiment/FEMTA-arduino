#include "PinCtrl.h"
#include "StateLogic.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "Arduino.h"
//include other libraries as needed

void openFlowValve(int flowOpenPin){

    digitalWrite(flowOpenPin, HIGH);
    delay(30);
    digitalWrite(flowOpenPin, LOW);

}

void closeFlowValve(int flowClosePin){

    digitalWrite(flowClosePin, HIGH);
    delay(30);
    digitalWrite(flowClosePin, LOW);
}

void openVentValve(int ventOpenPin){
    digitalWrite(ventOpenPin, HIGH);
    delay(30);
    digitalWrite(ventOpenPin, LOW);

}

void closeVentValve(int ventClosePin){

    digitalWrite(ventClosePin, HIGH);
    delay(30);
    digitalWrite(ventClosePin, LOW);
}

void signalStart(int startPin){

    digitalWrite(startPin, HIGH);
}

void signalEnd(int startPin){
    
    digitalWrite(startPin, LOW);
}