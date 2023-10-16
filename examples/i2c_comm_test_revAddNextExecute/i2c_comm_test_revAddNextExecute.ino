#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

int nextTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Accelerometer Test");
  Serial.println("");

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }
  Serial.println("card initialized.");
  
  accel.setRange(LSM303_RANGE_4G);
  lsm303_accel_range_t new_range = accel.getRange();

  accel.setMode(LSM303_MODE_NORMAL);
  lsm303_accel_mode_t new_mode = accel.getMode();
}

void loop() {
  // Check if we're ready to pull the next sensor data point
  if(nextTime <= millis()) {
    
    Serial.print('\n');
    Serial.print("Polling sensors");
    Serial.print('\n');
    Serial.print("Time: ");
    Serial.print(millis());
    Serial.print('\n');
    Serial.print("Execute time: ");
    Serial.print(nextTime);
    Serial.print('\n');
    Serial.print('\n');
    
    // make a string for assembling the data to log:
    String dataString = "";

    // read three sensors and append to the string:
    sensors_event_t event;
    accel.getEvent(&event);

    float f_rounded = round(millis() * 1000) / 1000000.;
    dataString += String(f_rounded) + ",";
    dataString += String(event.acceleration.x) + "," + String(event.acceleration.y) + "," + String(event.acceleration.z);
    dataString += "," + String(sqrt(pow(event.acceleration.x,2) + pow(event.acceleration.y,2) + pow(event.acceleration.z,2)));

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("AT07.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    // update timer:
    nextTime += 100;

  }
  else{
    Serial.print("Waiting to poll sensors");
    Serial.print('\n');
    Serial.print("Time: ");
    Serial.print(millis());
    Serial.print('\n');
    Serial.print("Execute time: ");
    Serial.print(nextTime);
    Serial.print('\n');
  }
 
}
