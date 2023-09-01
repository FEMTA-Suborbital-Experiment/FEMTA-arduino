
#include <SD.h>                      // Allows usage of SD Cards 




int chipSelect = 4;                  // Pin # for SD Card Select

File vibFile;                       // Global Declaration for file vairable
File outFile;
String fileName;


typedef struct accel_data {
  float ax;
  float ay;
  float az;
  float t;
} accel_data;

union {
  float fval;
  byte bval[4];
} float2byte;

accel_data buf;

void setup() {
  Serial.begin(115200);           // Opens Serial Communications
  // Initialize SD Card ---------------------------------------------------------------------------------------------

  // Wait to Connect to SD Card - - - - - - - - - - - - - - - - - - -
  while (!Serial) { ; }
  //delay(500);

  // Print Result to Console - - - - - - - - - - - - - - - - - - - - -
  //Serial.print("Initializing SD card... ");

  if (!SD.begin(chipSelect)) {
    Serial.print("ERROR!\n");
    Serial.println("SD Card failed or not found");
    Serial.println("Resetting Arduino due to Card Failure\n");
    delay(1000);
    setup();
    return;
  }
  //Serial.print("PASSED\n");

  // Open Data Log File ---------------------------------------------------------------------
  // Initialize Data Log File ---------------------------------------------------------------------------------------

  // Create txt file - - - - - - - - - - - - - - - - - - - - - - - - - 
  //Serial.print("Creating file... ");
  String fileType = ".txt";
  int i = 0;
  int fileCondition;
  
  do {
    fileName = i + fileType;              // Define file name
    if (SD.exists(fileName)) {            // If the file exist, i value plus one, and keep running the loop
      i = i + 1;
      fileCondition = 1;
    } 
    else {                                // If the file doesn't exist, create the file in write mode
      fileCondition = 0;                  // Kill the loop
    }
  } while(fileCondition);
  vibFile = SD.open((i-1)+fileType, FILE_READ);
  outFile = SD.open("log"+fileType, FILE_WRITE);

  int lines = 0;
  char c;
  while (lines < 7) {
    c = vibFile.read();
    Serial.print(c);
    if (c == '\n') {
      lines++;
    }
  }

  
}

void loop() {
  if (vibFile.available()) {
    vibFile.read((uint8_t *)&buf.ax, sizeof(buf.ax));
    vibFile.read((uint8_t *)&buf.ay, sizeof(buf.ay));
    vibFile.read((uint8_t *)&buf.az, sizeof(buf.az));
    vibFile.read((uint8_t *)&buf.t, sizeof(buf.t));

    /*
    outFile.print(buf.ax, 6);
    outFile.print('\t');
    outFile.print(buf.ay,6);
    outFile.print('\t');
    outFile.print(buf.az,6);
    outFile.print('\t');
    outFile.print(buf.t,6);
    outFile.print('\n');
    */
    
    Serial.print(buf.ax, 6);
    Serial.print('\t');
    Serial.print(buf.ay,6);
    Serial.print('\t');
    Serial.print(buf.az,6);
    Serial.print('\t');
    Serial.print(buf.t,6);
    Serial.print('\n');
    //delay(50);
    
  }

}

















