#include "PinCtrl.h"

PinCtrl::PinCtrl(
  int flowClose1,
  int flowOpen1,  
  int flowClose2,
  int flowOpen2,  
  int ventClose, 
  int ventOpen, 
  int signal
  ) 
  : closeSV1{flowClose1}, 
  openSV1{flowOpen1}, 
  closeSV2{flowClose2}, 
  openSV2{flowOpen2}, 
  closeLV1{ventClose}, 
  openLV1{ventOpen}, 
  signalPin{signal}
{
}

int PinCtrl::init() {
    closeFlowValve();
    openVentValve();

    return 0;
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