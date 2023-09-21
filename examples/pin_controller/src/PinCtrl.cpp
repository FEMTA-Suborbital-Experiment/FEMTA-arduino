#include "PinCtrl.h"

const float t_delay{50};

/**
 * @brief Construct a PinController class with the pins corresponding to a
 * valve or signal on the FEMTA Suborbital Experiment. The flow and vent
 * pins correspond to valves on the Propellant Management Experiment, while
 * the signal pin is an output which is measured by the FEMTA Thruster Experiment
 * and determines whether the Thruster Experiment should begin at the right time.
 * 
 * @param flowClose1 
 * @param flowOpen1 
 * @param flowClose2 
 * @param flowOpen2 
 * @param ventClose 
 * @param ventOpen 
 * @param signal 
 */
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


/**
 * @brief Initialize the pin modes and set the initial state of the valves
 * at pre-flight.
 * 
 * @return int 
 */
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

/**
 * @brief An overridden Protothread method which acts as a state machine that
 * checks if a certain pin is turned on and sets the pin to HIGH. After a certain
 * time delay t_delay, set the pin to LOW.
 * 
 * @return true 
 * @return false 
 */
bool PinCtrl::Run() {
  static float lastTime = 0;
  PT_BEGIN();

    for(;;) {
      lastTime = millis();
       if (closeSV1_bool) {
        digitalWrite(closeSV1, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        closeSV1_bool = 0;
        digitalWrite(closeSV1, LOW);
      }
      if (closeSV2_bool) {
        digitalWrite(closeSV2, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        closeSV1_bool = 0;
        digitalWrite(closeSV2, LOW);
      }
      if (openSV1_bool) {
        digitalWrite(openSV1, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        closeSV1_bool = 0;
        digitalWrite(openSV1, LOW);
      }
      if (openSV2_bool) {
        digitalWrite(openSV2, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        closeSV1_bool = 0;
        digitalWrite(openSV2, LOW);
      }
      if (closeLV1_bool) {
        digitalWrite(closeLV1, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        closeLV1_bool = 0;
        digitalWrite(closeLV1, LOW);
      }
      if (openLV1_bool) {
        digitalWrite(openLV1, HIGH);
        PT_WAIT_UNTIL(millis() - lastTime > t_delay);
        lastTime = millis();
        openLV1_bool = 0;
        digitalWrite(openLV1, LOW);
      }

    }

  PT_END();
}

/**
 * @brief Open flow valve 1
 * 
 */
void PinCtrl::openFlowValve1() {
  if (!openSV1_bool && !closeSV1_bool)
    openSV1_bool = 1;
}

/**
 * @brief Close flow valve 1
 * 
 */
void PinCtrl::closeFlowValve1() {
  if (!closeSV1_bool && !openSV1_bool)
    closeSV1_bool = 1;
}

/**
 * @brief Open flow valve 2
 * 
 */
void PinCtrl::openFlowValve2() {
  if (!openSV2_bool && !closeSV2_bool)
    openSV2_bool = 1;
}

/**
 * @brief Close flow valve 2
 * 
 */
void PinCtrl::closeFlowValve2(){
  if (!closeSV2_bool && !openSV2_bool)
    closeSV2_bool = 1;
}

/**
 * @brief Open vent valve 1
 * 
 */
void PinCtrl::openVentValve(){
  if (!openLV1_bool && !openLV1_bool)
    openLV1_bool = 1;
}

/**
 * @brief Close vent valve 1
 * 
 */
void PinCtrl::closeVentValve() {
  if (!closeLV1_bool && !openLV1_bool)
    closeLV1_bool = 1;
}

/**
 * @brief Signal the start of an experiment
 * 
 */
void PinCtrl::signalStart(){
  digitalWrite(signalPin, HIGH);
}

/**
 * @brief Signal to stop the experiment
 * 
 */
void PinCtrl::signalStop(){
  digitalWrite(signalPin, LOW);
}