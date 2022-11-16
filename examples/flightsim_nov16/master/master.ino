//this is the code for the board that is going to read values from the other two
//sensor 1 is accelleration data, 2 is temperature data

#include <Wire.h>
int state = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
//......int index = 0;
const int REMOTE = 8;
const int LOCAL = 4;
const int VIBRATION_MAG = 10;
const int BUFFER_SIZE = 30;

String StringData = "";
int newVar = 0;
float pressThresh[4] = {85000, 0.1, .0008, .00175}; // (kpa) ground, MECO, coast-start, coast-end CALIBRATED ON HIGH END
int accelThresh = 5; //m/s^2 UNCALIBRATED
int lastState = 0;
int pressFlag = 0;
int accelFlag = 0;
int timeFlag = 0;
int stateFlag = 0; //0 = pre-liftoff, 1 = ascent, 2 = MECO, 3 = descent, 4 = post-descent
int i = 0;
int j = 0;

float sum[6]; //1 - pressure new, 2 - pressure old, 3 - accel new, 4 - accel old, 5 - stdv accel new, 6 - stdv accel old
float average[4]; //same indexing as sum

float stdv1[BUFFER_SIZE]; //1 - accel new, 2 - accel old
float stdv2[BUFFER_SIZE];
float avstdv[2]; //1 - accel new, 2 - accel old

float accelBuff1[BUFFER_SIZE];
float accelBuff2[BUFFER_SIZE];
float temp[2];


void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // join I2C bus with address 4
  Serial.begin(9600); // 9600 bits per second
  //Wire.onReceive(receiveEvent);
  
}


float vibration(int vibrationMag) {
  return rand() % vibrationMag + 0.1 * (rand() % 10);
}


void loop() {
  Wire.requestFrom(REMOTE, 30*sizeof(char)); // 2nd param is exact size of thing being received
  StringData = "";
  while (Wire.available()){
    char c = Wire.read();
    StringData.concat(c);
  }
  int index = StringData.indexOf(",");
  String Pressure = StringData.substring(0,index);
  float pressure = Pressure.toFloat();
  Serial.print("pressure: ");
  Serial.print(pressure);
  int indexEnd = StringData.indexOf(";");
  String Accel = StringData.substring(index+1, indexEnd);
  float accel = Accel.toFloat() + vibration(VIBRATION_MAG);
  Serial.print(" accel: ");
  Serial.println(accel); // done printing raw data

  // Data Smoothing and Flight Phase Detection
  float temp = accel;
  float average[2] = {0,0}; // First is newer buffer, second is older
  for (j = 29; j >= 1; j--)
  {
     accelBuff1[j] = accelBuff1[j - 1]; //Slide all the buffer 1 point to the right for newer buffer
     accelBuff2[j] = accelBuff2[j - 1]; // Same as above but with older buffer
  }
  accelBuff1[0] = temp; // Set first point of new buffer to newly aquired accelerometer data
  temp = accelBuff1[29];
  accelBuff2[0] = temp; // Set first value of older buffer to last value of newer buffer (sliding)
  for (j = 0; j < 30; j++)
  {
    average[0] += accelBuff1[j];
    average[1] += accelBuff2[j];
  }
  average[0] /= 30;
  average[1] /= 30;
  Serial.println("Average 0: " + String(average[0]));
  Serial.println("Average 1: " + String(average[1]));
  if ((average[0] - average[1] >= accelThresh) && lastState != 1)
  {
    Serial.println("Entered Liftoff at index: " + String(i));
    lastState = 1;
  }
  else if ((average[1] - average[0] >= accelThresh) && lastState != 2)
  {
    Serial.println("Entered MECO at index: " + String(i));
    lastState = 2;
  }  
  
  average[1] = 0;
  average[2] = 0;
  
  delay(1000);
}
