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
  closeSV1_bool = 0;
  openSV1_bool = 0;
  closeSV2_bool = 0;
  openSV2_bool = 0;
  closeSV1_bool = 0;
  closeSV1_bool = 0;
}

int PinCtrl::init() {
    pinMode(closeSV1, OUTPUT);
    pinMode(closeSV2, OUTPUT);
    pinMode(openSV1, OUTPUT);
    pinMode(openSV2, OUTPUT);
    pinMode(closeLV1, OUTPUT);
    pinMode(openLV1, OUTPUT);
    
    closeFlowValve1();
    closeFlowValve2();
    openVentValve();

    return 0;
}

bool PinCtrl::Run() {
  PT_BEGIN();

    for(;;) {
      if (closeSV1_bool) {
        digitalWrite(closeSV1, HIGH);
        PT_YIELD();
        closeSV1_bool = 0;
        digitalWrite(closeSV1, LOW);
      }
      if (closeSV2_bool) {
        digitalWrite(closeSV2, HIGH);
        PT_YIELD();
        closeSV1_bool = 0;
        digitalWrite(closeSV2, LOW);
      }
      if (openSV1_bool) {
        digitalWrite(openSV1, HIGH);
        PT_YIELD();
        closeSV1_bool = 0;
        digitalWrite(openSV1, LOW);
      }
      if (openSV2_bool) {
        digitalWrite(openSV2, HIGH);
        PT_YIELD();
        closeSV1_bool = 0;
        digitalWrite(openSV2, LOW);
      }
      if (closeLV1_bool) {
        digitalWrite(closeLV1, HIGH);
        PT_YIELD();
        closeLV1_bool = 0;
        digitalWrite(closeLV1, LOW);
      }
      if (openLV1_bool) {
        digitalWrite(openLV1, HIGH);
        PT_YIELD();
        openLV1_bool = 0;
        digitalWrite(openLV1, LOW);
      }

    }

  PT_END();
}

void PinCtrl::openFlowValve1() {
  if (!openSV1_bool && !closeSV1_bool)
    openSV1_bool = 1;
}

void PinCtrl::closeFlowValve1() {
  if (!closeSV1_bool && !openSV1_bool)
    closeSV1_bool = 1;
}

void PinCtrl::openFlowValve2() {
  if (!openSV2_bool && !closeSV2_bool)
    openSV2_bool = 1;
}

void PinCtrl::closeFlowValve2(){
  if (!closeSV2_bool && !openSV2_bool)
    closeSV2_bool = 1;
}

void PinCtrl::openVentValve(){
  if (!openLV1_bool && !openLV1_bool)
    openLV1_bool = 1;
}
void PinCtrl::closeVentValve() {
  if (!closeLV1_bool && !openLV1_bool)
    closeLV1_bool = 1;
}

void PinCtrl::signalStart(){
  digitalWrite(signalPin, HIGH);
}

void PinCtrl::signalStop(){
  digitalWrite(signalPin, LOW);
}