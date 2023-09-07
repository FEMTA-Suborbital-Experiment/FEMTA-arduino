#include "PinCtrl.h"

PinCtrl::PinCtrl(int flowOpen, int flowClose, int ventOpen, int ventClose, int signal) 
  : flowOpenPin{flowOpen}, flowClosePin{flowClose}, ventOpenPin{ventOpen}, signalPin{signal}
{

}

PinCtrl::init() {
    closeFlowValve();
    openVentValve();
}

void PinCtrl::openFlowValve(){

}

void PinCtrl::closeFlowValve(){

}

void PinCtrl::openVentValve(){

}
void PinCtrl::closeVentValve(){

}

void PinCtrl::signalStart(){

}

void PinCtrl::signalStop(){

}