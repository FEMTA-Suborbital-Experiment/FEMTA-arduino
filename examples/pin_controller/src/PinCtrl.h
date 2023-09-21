#ifndef FEMTA_PIN_CONTROLLER
#define FEMTA_PIN_CONTROLLER

#include <Protothread.h>
#include "Arduino.h"

/**
 * @brief Pin Controller class
 * 
 */
class PinCtrl : public Protothread {
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

    void openFlowValve1();
    void closeFlowValve1();
    void openFlowValve2();
    void closeFlowValve2();
    void openVentValve();
    void closeVentValve();
    void signalStart();
    void signalStop();

    int init();

    bool Run();

  private:
    int closeSV1; 
    int openSV1;
    int closeSV2; 
    int openSV2;
    int closeLV1; 
    int openLV1;
    int signalPin;

    bool closeSV1_bool; 
    bool openSV1_bool;
    bool closeSV2_bool; 
    bool openSV2_bool;
    bool closeLV1_bool; 
    bool openLV1_bool;

    void togglePin(int pin);
    
};

#endif
