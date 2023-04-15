#ifndef PinCtrl
#define PinCtrl

#include "Arduino.h"

class PinCtrl{
  public:
    PinCtrl(int flowOpenPin, int flowClosePin, int ventOpenPin, int ventClosePin, int startPin);
    
  private:
    void openFlowValve(int flowOpenPin);
    void closeFlowValve(int flowClosePin);
    void openVentValve(int ventOpenPin);
    void closeVentValve(int ventClosePin);
    void signalStart(int startPin);
}

#endif
