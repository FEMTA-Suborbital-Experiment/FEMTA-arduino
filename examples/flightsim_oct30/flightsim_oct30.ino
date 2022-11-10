#include <CSV_Parser.h>

#include <SPI.h>
#include <SD.h>

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

float stdv1[30]; //1 - accel new, 2 - accel old
float stdv2[30];
float avstdv[2]; //1 - accel new, 2 - accel old

float accelBuff1[30];
float accelBuff2[30];
float temp[2];

const int chipSelect = 4;

void setup() {
  Serial.begin(9600); // 9600 bits per second
  // put your setup code here, to run once:
  
  Serial.println("Initializing card");
  if(!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    
    // don't do anything more:
    while (1);
  }
  Serial.println("Card initialized");

}

void loop() {
  CSV_Parser cp(/*format*/ "ffff", /*has_header*/ true, /*delimiter*/ ',');

  if(cp.readSDfile("FSDO.CSV")) {
    float *altitude = (float*)cp["altitude"];
    float *temperature = (float*)cp["temperature"];
    float *pressure = (float*)cp["pressure"];
    float *acceleration = (float*)cp["acceleration"];

    float temp = acceleration[i];
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
  
    //Serial.println(g_cp.getRowsCount());
    Serial.println("Altitude: " + String(altitude[i]));
    Serial.println("Temperature: " + String(temperature[i]));
    Serial.println("Pressure: " + String(pressure[i]));
    Serial.println("Acceleration: " + String(acceleration[i]));
    
    average[1] = 0;
    average[2] = 0;
    
    if (i == cp.getRowsCount()) {
      printf("\n\nDone!\n\n");
      return;
    }
    i++;
    delay(1000);
    } else {
      Serial.println("Can't detect csv");
    }
}
