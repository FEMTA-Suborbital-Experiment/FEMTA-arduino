#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  int vibration;
  int i = 0;
  int j = 0;
  int lastState = 0;   // Make it 1 if in liftoff and 2 if in MECO
  int accel[300];      // mock acceleration data
  int accelThresh = 5; // m/s^2 UNCALIBRATED
  float accelBuff1[30];
  float accelBuff2[30];
  float temp;
  printf("\nEnter the magnitude of vibration (integer): ");
  scanf("%d", &vibration);
  printf("\nVibration Magnitude = %d\n", vibration);
  float average[2] = {0, 0}; // First is newer buffer, second is older
  for (i = 0; i <= 100; i++) {
    accel[i] = 9 + rand() % vibration + 0.1 * (rand() % 10);
  }
  for (i = 101; i <= 200; i++) {
    accel[i] = 22 + rand() % vibration + 0.1 * (rand() % 10);
  }
  for (i = 201; i < 300; i++) {
    accel[i] = -1 + rand() % vibration + 0.1 * (rand() % 10);
  }
  // Create fake accelerometer data where first 100 points are before takeoff,
  // next 100 are takeof, next 100 are MECO
  for (i = 0; i < 270; i++) {
    temp = accel[i]; // Simulating getting new data
    if (i < 30) {
      for (j = i; j >= 1; j--) {
        accelBuff1[j] = accelBuff1[j - 1];
      }
      accelBuff1[0] = temp;
    } else if (i < 60) {
      for (j = 29; j >= 1; j--) {
        accelBuff1[j] = accelBuff1[j - 1];
      }
      accelBuff1[0] = temp;
      temp = accelBuff1[29];
      for (j = i - 30; j >= 1; j--) {
        accelBuff2[j] = accelBuff2[j - 1];
      }
      accelBuff2[0] = temp;
    } else {
      for (j = 29; j >= 1; j--) {
        accelBuff1[j] = accelBuff1[j - 1]; // Slide all the buffer points to the
                                           // right for the newer buffer.
        accelBuff2[j] =
            accelBuff2[j - 1]; // Same as above but with the older buffer
      }
      accelBuff1[0] = temp; // Set first point of new buffer to newly aquired
                            // accelerometer data
      temp = accelBuff1[29];
      accelBuff2[0] = temp; // Set first value of older buffer to last value of
                            // newer buffer (sliding)
    }
    if (i > 60) {
      for (j = 0; j < 30; j++) {
        average[0] += accelBuff1[j];
        average[1] += accelBuff2[j];
      }
      average[0] /= 30;
      average[1] /= 30;
      if ((average[0] - average[1] >= accelThresh) && lastState != 1) {
        printf("\nEntered Liftoff at index %d", i);
        if (i < 100){
          printf(": this is a phantom detection of liftoff.");
        }
        lastState = 1;
      }
      else if ((average[1] - average[0] >= accelThresh) && lastState != 2) {
        printf("\nEntered MECO at index %d", i);
        if (i < 200){
          printf(": this is a phantom detection of MECO");
        }
        lastState = 2;
      }

      average[1] = 0;
      average[2] = 0;
    }
  }
  printf("\n\nDone!\n\n");
  return 0;
}

//Please note that Liftoff should occur at index 100 and MECO should occur at index 200. If detection occurs slightly past these points, that is fine as the buffer method will cause slight delay in detection. If a detection for a phase occurs before it's corresponding point, it is a Phantom Detection, meaning the vibration value you inputted is too high and the buffer method falsely detected a phase change