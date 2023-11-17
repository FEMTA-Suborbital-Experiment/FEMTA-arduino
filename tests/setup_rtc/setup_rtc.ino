#include <StateLogic.h>
StateLogic rtc_test;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect.
    }

    Serial.print("Initializing SD card...");

    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        while (1);
    }
    Serial.println("initialization done.");


    if (!rtc_test.rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) delay(10);
    }

    bool initialized = rtc_test.init_rtc("test");
    if(!initialized) {
      Serial.println("Unable to Initialize RTC Start Time!");
    }
    
    //Reading from the file containing the rtc start time to confirm it was initialized correctly
    uint32_t test_init_start = 0;
    File startFile = SD.open("test.dat", FILE_READ);

    if (startFile) {
      Serial.print("Reading from file...");
      startFile.read((uint8_t *)&(test_init_start), sizeof(test_init_start));
      startFile.close();
      Serial.println("done.");
    } else {
      Serial.println("Error reading to file");
    }

    Serial.print("Start Time Variable:");
    Serial.println(rtc_test.start_time);

    Serial.print("Read Start Time Variable:");
    Serial.println(test_init_start);

    Serial.println("Clearing start_time variable.");
    rtc_test.start_time = 0;
    Serial.println("Clearing read start_time variable.");
    test_init_start = 0;

    Serial.println("Re-initializing the rtc");
    bool reinitialized = rtc_test.reinit_rtc();
    if(!reinitialized) {
      Serial.println("Unable to Initialize RTC Start Time!");
    }

    //Reading from the file containing the rtc start time to confirm can find the original start time
    startFile = SD.open("test.dat", FILE_READ);
    if (startFile) {
      Serial.print("Reading from file...");
      startFile.read((uint8_t *)&(test_init_start), sizeof(test_init_start));
      startFile.close();
      Serial.println("done.");
    } else {
      Serial.println("Error reading to file");
    }

    Serial.print("Reinitialized Start Time Variable:");
    Serial.println(rtc_test.start_time);

    Serial.print("Reinitialized Read Start Time Variable:");
    Serial.println(test_init_start); 

    Serial.println("Now Printing Elapsed Time Since Start:");
}

void loop() {
  uint32_t elapsed_time = rtc_test.elapsed_time();
  Serial.print("Elapsed Time: ");
  Serial.println(elapsed_time);
  delay(1000);
}