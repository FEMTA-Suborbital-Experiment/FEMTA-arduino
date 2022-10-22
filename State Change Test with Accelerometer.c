#include <stdio.h>
#include <math.h>
int main(void) {
  int i = 0;
  int j = 0;
  int lastState = 0; // Make it 1 if in liftoff and 2 if in MECO
  int accel[300]; // mock acceleration data
  int accelThresh = 5; //m/s^2 UNCALIBRATED
  float accelBuff1[30];
  float accelBuff2[30];
  for (i = 0; i < 30; i++)
    {
      accelBuff1[i] = 9.8;
      accelBuff2[i] = 9.8;
    } // Presetting values for buffer
  float temp;
  float average[2] = {0,0}; // First is newer buffer, second is older
  for (i = 0; i <= 100; i++) 
  {
    accel[i] = 9.0; //First 100 data points are at 9 to simulate before takeoff
  }
   for (i = 101; i <= 200; i++)
  {
    accel[i] = 25; //Next 100 data points are at 25 to simulate takeoff
  }
  for (i = 201; i < 300; i++)
  {
    accel[i] = 0; //Last 100 data points are at 0 to simulate MECO
  }  
  for (i = 30; i < 270; i++)
    {
      temp = accel[i]; // Simulating getting new data
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
      if ((average[0] - average[1] >= accelThresh) && lastState != 1)
      {
        printf("\nEntered Liftoff at index %d", i);
        lastState = 1;
      }
      else if ((average[1] - average[0] >= accelThresh) && lastState != 2)
      {
        printf("\nEntered MECO at index %d", i);
        lastState = 2;
      }  
      
      average[1] = 0;
      average[2] = 0;
    }
  printf("\n\nDone!\n\n");
  return 0;
}
