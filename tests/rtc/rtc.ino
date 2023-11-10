// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
//#include <StateLogic.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup () {
  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power or is on new device. Initializing time to compile time");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
    //Getting Start Time
    DateTime start = rtc.now();
    delay(1000);

    DateTime now = rtc.now();
    
    uint8_t elapsed_hour;
    uint8_t elapsed_min;
    uint8_t elapsed_sec;

    elapsed_hour = now.hour() - start.hour();
    elapsed_min = now.minute() - start.minute();
    elapsed_sec = now.second() - start.second();

    Serial.print("Time: ");
    Serial.print(elapsed_hour, DEC);
    Serial.print(':');
    Serial.print(elapsed_min, DEC);
    Serial.print(':');
    Serial.print(elapsed_sec, DEC);
    Serial.println();

    Serial.println();
    delay(1000);
}
