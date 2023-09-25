/**
 * @file PinCtrl.h
 * @author Jacob Valdez (valdez24@purdue.edu)
 * @brief The FEMTA Pin Controller class is responsible for beginning the 
 * FEMTA Suborbital Experiments. The Pin Controller controls the valves 
 * on the propellant tank and the collection chamber to allow the flow of
 * propellant from the tank to the chamber, and it sends the signal to the
 * FEMTA Thruster datalogger to begin its experiment.
 * 
 * The Pin Controller makes use of a C++ port of Adam Dunkels' Protothreads.
 * Protothreads enable the Pin Controller to set the pins outside of the
 * main execution sketch of the flight simulation or FEMTA experiment.
 * This allows the pins to be impulsed (set a pin voltage HIGH, wait a few
 * milliseconds, and set teh pin voltage LOW), avoiding the possibility
 * of pausing the main program's execution. 
 * 
 * @date 2023-09-25
 * 
 * 
 */

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

    int init(bool willInitializeValves=0);

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
