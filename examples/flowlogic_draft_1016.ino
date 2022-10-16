int newVar = 0;
int Pressure[30] = {101250, 101250, 101250, 101000, 100500, 100000, 99000, 98000, 95000, 90000, 85000, 75000, 50000, 40000, 30000, 15000, 5000, 500, 50, 1, 0.2, 0.08, 0.001, 0.0005, 0.0005, 0.0005, 0.0005, 0.08, 0.5, 500}; // mock pressure data, 25 data points
int Temperature[25] = {90,88,83,80,76,71,64,60,52,40,31,20,13,13,19,26,34,45,58,63,71,84,89,90,90}; // mock temperature data
int accel[30] = { 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // mock acceleration data
int pressThresh[4] = {85000, 0.1, .0008, .00175}; // (kpa) ground, MECO, coast-start, coast-end CALIBRATED ON HIGH END
int accelThresh = 5; //m/s^2 UNCALIBRATED
int pressFlag = 0;
int accelFlag = 0;
int timeFlag = 0;
int stateFlag = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
int index = 0;
int j = 0;
float sum[6]; //1 - pressure new, 2 - pressure old, 3 - accel new, 4 - accel old, 5 - stdv accel new, 6 - stdv accel old
float average[4]; //same indexing as sum
float stdv1[30]; //1 - accel new, 2 - accel old
float stdv2[30];
float avstdv[2]; //1 - accel new, 2 - accel old
float pressBuff1[30];
float pressBuff2[30];
float accelBuff1[30];
float accelBuff2[30];
float temp[2];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // 9600 bits per second
}
void loop() {

  float accelValue;
  float pressValue;
  
  // put your main code here, to run repeatedly:
  //this is just to make sure the board is functioning when we get no output
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(10);                       // wait for a second

  //BUFFER LOGIC

  //read in value
  accelValue = 6.5; //Change to be latest accel value
  pressValue = 50000; //Change to be latest press value


  //Buffer update logic
  temp[1] = pressBuff1[30]
  for (i = 30; i--; i > 1){
    pressBuff1[i] = pressBuff1[i - 1]
  }
  pressBuff1[1] = pressValue;
  for (i = 30; i--; i > 1){
    pressBuff2[i] = pressBuff2[i - 1]
  }
  pressBuff2[1] = temp[1];

  
  temp[2] = accelBuff1[30]
  for (i = 30; i--; i > 1){
    accelBuff1[i] = accelBuff1[i - 1]
  }
  accelBuff1[1] = accelValue;
  for (i = 30; i--; i > 1){
    accelBuff2[i] = accelBuff2[i - 1]
  }
  accelBuff2[1] = temp[1];
  //Buffer update logic


  //Buffer average/stdv logic
  sum = 0;
  for (i = 1; i++; i <= 30){
    sum[1] += pressBuff1[i];
    sum[2] += pressBuff2[i];
    sum[3] += accelBuff1[i];
    sum[4] += accelBuff2[i];
  }

  average[1] = sum[1] / 30;
  average[2] = sum[2] / 30;
  average[3] = sum[3] / 30;
  average[4] = sum[4] / 30;

  for (i = 1; i++; i <= 30){
    stdv1[i] = pow((accelBuff1[i] - average[3]), 2);
    stdv2[i] = pow((accelBuff2[i] - average[4]), 2);
  }

  for (i = 1; i++; i <= 30){
    sum[5] += stdv1[i];
    sum[6] += stdv2[i];
  }

  avstdv[1] = sum[5] / 30;
  avstdv[2] = sum[6] / 30;

  
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
  if (average[3] - average[4] >= accelThresh) {
    accelFlag = 1;
  }
  else if(average[4] - average[3] >= accelThresh) {
    accelFlag = 0;  
  }

  //add timeflag switching

  
  //this is super rudimentary, going to make decisions based on data trends in final model
  //DO NOT ACTIVATE until buffers are filled
  if (accelFlag == 0 && pressFlag == 0){
    state = 0;
    //pre-liftoff
      //no data logging
      //should be no valve ops, closed flow valve
    Serial.print("pre-liftoff\n");
  }
  else if (accelFlag == 1 && pressFlag == 1 && state == 0){
    state = 1;
    //liftoff()
      //start data logging
      //no valve ops
    Serial.print("ascent\n");
  }
  //ADD TIME LOGIC
  else if (accelFlag == 1 && pressFlag == 2 && state == 1){
    state = 2;
    //meco()
      //no ops, just a pre-coast state
    Serial.print("MECO\n");
  }
  //ADD TIME SINCE MECO LOGIC
  else if (pressFlag == 3 && state == 2){
    state = 3;
    //coast()
      //open flow valve, cc valve
      //log flow data
    Serial.print("coast start\n");
  }
  else if (accelFlag == 1 && state == 3){ // && time passed ~ 160sec
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
