int newVar = 0;
int Pressure[25] = {10,10,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,10}; // mock pressure data, 25 data points
int Temperature[25] = {90,88,83,80,76,71,64,60,52,40,31,20,13,13,19,26,34,45,58,63,71,84,89,90,90}; // mock temperature data
int accel[25] = { 0, 4, 9, 9, 9, 7, 7, 7, 7, 8, 4, 2, 0, 0, 2, 3, 4, 4, 3, 3, 4, 6, 9, 4, 0}; // mock acceleration data
int pressThresh[4] = {85000, 0.1, .0008, .00175}; //ground, MECO, coast-start, coast-end CALIBRATED
int accelThresh[2] = [1, 5]; //low, high UNCALIBRATED
int pressFlag = 0;
int accelFlag = 0;
int timeFlag = 0;
int stateFlag = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
int index = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // 9600 bits per second
}
void loop() {
  // put your main code here, to run repeatedly:
  //this is just to make sure the board is functioning when we get no output
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(10);                       // wait for a second


  //CONSIDER: break to flag check function, called on every loop
  //pressflag decision
  if (pressure[index] < pressThresh[2]){
    pressFlag = 1;
  }
  else if (pressure[index] < pressThresh[3] && pressFlag == 1){
    pressFlag = 2;
  }
  else if (pressure[index] < pressThresh[4] && pressFlag == 2){
    pressFlag = 3;
  }

  //accelflag decision
  if (accel[index] < accelThresh[1]) {
    accelFlag = 0;
  }
  else if (accel[index] > accelThresh[2] {
    accelFlag = 1;  
  }

  //add timeflag switching

  
  //this is super rudimentary, going to make decisions based on data trends in final model
  if (accelFlag == 0 && pressFlag == 0){
    state = 0;
    //pre-liftoff
      //no data logging
      //should be no valve ops, closed flow valve
    Serial.print("pre-liftoff\n");
  }
  else if (accelFlag == 2 && pressFlag == 1 && state == 0){
    state = 1;
    //liftoff()
      //start data logging
      //no valve ops
    Serial.print("ascent\n");
  }
  //ADD TIME LOGIC
  else if (accelFlag == 1 && pressFlag == 2){
    state = 2;
    //meco()
      //no ops, just a pre-coast state
    Serial.print("MECO\n");
  }
  //ADD TIME SINCE MECO LOGIC
  else if (pressFlag == 3){
    state = 3;
    //coast()
      //open flow valve, cc valve
      //log flow data
    Serial.print("coast start\n");
  }
  else if (accelFlag == 2 && pressFlag == 4){
    state = 4;
    //endcoast()
      //stop logging?
      //close flow?
    Serial.print("coast end\n");
  }
  else{
    Serial.print("unsure of state\n");
  }
  index++;
  delay(1000);
}
