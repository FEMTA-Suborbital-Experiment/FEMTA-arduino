#ifndef FEMTA_PIN_CONTROLLER
#define FEMTA_PIN_CONTROLLER

#include "Arduino.h"

class PinCtrl{
  public:
    PinCtrl(int flowOpen, int flowClose, int ventOpen, int ventClose, int signal);

    void openFlowValve();
    void closeFlowValve();
    void openVentValve();
    void closeVentValve();
    void signalStart();
    void signalStop();

    void init();

  private:
    int flowOpenPin;
    int flowClosePin; 
    int ventOpenPin;
    int ventClosePin; 
    int signalPin;
    
}

#endif
