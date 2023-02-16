#include <SPI.h>
#include <SD.h>

#define CARD_SELECT 4
#define MAX_VOLTAGE 3.30
#define FLAG_TOLERANCE 20
#define MAX_VALUE 1023
#define MIN_VALUE 0
#define STEP_SIZE 1

const int serial_precision = 5;
const int log_precision = 10;

int write_value = 0;
int count = 0;

File logfile;

void setup() {
  Serial.begin(9600);
  pinMode(A5, INPUT);
  
  if (!SD.begin(CARD_SELECT)) {
    Serial.println("Card init. failed!");
  }
  char filename[15];
  strcpy(filename, "/ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    if (! SD.exists(filename)) {
      break;
    }
  }
  logfile = SD.open(filename, FILE_WRITE);
  if(!logfile) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
  }
  logfile.println("time(msec)\tInput Voltage(V)");
}

// the loop routine runs over and over again forever:
void loop() {

  // TODO: Rewrite section below into a function and reduce while loops
  // tuning system
  if (!hasFlag()) { //does not see flag
    count = 0;
    while (true) {
      int dec_count = 0;
      while (write_value > MIN_VALUE) {
        if (hasFlag()) {
          return;
        }

        if (dec_count < 0) {
          write_value -= STEP_SIZE;
          dec_count = 20;
        } else {
          dec_count -= 1;
        }

        writeVoltage();
        Serial.print("Decreasing: ");
        Serial.println(convert2voltage(write_value), serial_precision);

        delay(1);
      }
      int inc_count = 0;
      while (write_value < MAX_VALUE) {
        if (hasFlag()) {
          return;
        }

        if (inc_count < 0) {
          write_value += STEP_SIZE;
          writeVoltage();
          inc_count = 20;
        } else {
          inc_count -= 1;
        }

        Serial.print("Increasing: ");
        Serial.println(convert2voltage(write_value), serial_precision);
        
        delay(1);
      }
    }
  } 

  // TODO: Test separate adalogger implementation
  // manually flush to the files which takes around 20~40 msec
  logfile.flush();

  // artifical kick to the system
  if (count < 0) {
    write_value += STEP_SIZE;
    count = 5;
  } else {
    count -= 1;
  }
  writeVoltage();
  Serial.print("Write value: ");
  Serial.println(write_value);
  Serial.print("Input Voltage (V): ");
  Serial.println(convert2voltage(write_value), serial_precision);

  // delay of the system
  //  delay(30);
}

bool hasFlag() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A5);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print("Sensor Reading: ");
  Serial.println(sensorValue);
  return sensorValue > FLAG_TOLERANCE;
}

void writeVoltage() {
  if (write_value < 0) {
    write_value = 0;
  } else if (write_value > MAX_VALUE) {
    write_value = MAX_VALUE;
  }
  analogWrite(A0, write_value);

  // write to the file
  logfile.print(millis());
  logfile.print("\t");
  logfile.println(write_value, log_precision);
  return;
}

float convert2voltage(int value) {
  return MAX_VOLTAGE * write_value / MAX_VALUE;
}
