// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
//#include <StateLogic.h>
#include <RTClib.h>

RTC_DS3231 rtc;
DateTime start;

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

  start = rtc.now();
  //rtc.adjust(start);
}

void loop () {
    DateTime now = rtc.now();
    
    // Calculate and print Unix time
    unsigned long unixTime = now.unixtime() - start.unixtime();
    Serial.print("Elapsed Unix time: ");
    Serial.println(unixTime);
    
    delay(1000);
}
