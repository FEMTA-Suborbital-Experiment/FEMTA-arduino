#ifndef FEMTA_PIN_CONTROLLER
#define FEMTA_PIN_CONTROLLER

#include "Arduino.h"

class PinCtrl{
  public:
    PinCtrl(
      int flowClose1, 
      int flowOpen1, 
      int flowClose2, 
      int flowOpen2, 
      int ventOpen, 
      int ventClose, 
      int signal
      );

    void openFlowValve();
    void closeFlowValve();
    void openVentValve();
    void closeVentValve();
    void signalStart();
    void signalStop();

    int init();

  private:
    int closeSV1; 
    int openSV1;
    int closeSV2; 
    int openSV2;
    int closeLV1; 
    int openLV1;

    int signalPin;
    
};

#endif
