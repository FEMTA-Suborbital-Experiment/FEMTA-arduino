#include <SPI.h>
#include <SD.h>
#include <math.h>

#define CARD_SELECT 4
#define MAX_VOLTAGE 3.30
#define FLAG_TOLERANCE 20
#define MAX_VALUE 1023
#define MIN_VALUE 0
#define STEP_SIZE 1

const int serial_precision = 5;
const int log_precision = 10;

int write_value = 0;
int inc_count = 0;
int dec_count = 0;
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
  
    if (write_value < MAX_VALUE) {

      // Increase d'Arsonval voltage if  
      if (inc_count < 0) {
        write_value += STEP_SIZE;
        inc_count = 20;
      } else {
        inc_count -= 1;
      }

      Serial.print("Increasing: ");

    } else {
        // Decrease d'Arsonval
      if (dec_count < 0) {
        write_value -= STEP_SIZE;
        dec_count = 20;
      } else {
        dec_count -= 1;
      }

      Serial.print("Decreasing: ");
      
    }
  
  delay(1);
  } else {

    // TODO: Test separate adalogger implementation
    // manually flush to the files which takes around 20~40 msec
    logfile.flush();
  }

  // artifical kick to the system to ensure that the needle does not remain in one place due
  // to sticking friction (causes needle to wobble)
  perturbFlag();

  writeVoltage();
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


void perturbFlag() {
  if (count < 0) {
    write_value += STEP_SIZE;
    Serial.print("with artificial kick: ");
    count = 5;
  } else {
    count -= 1;
  }
}


void writeVoltage() {
  write_value = constrain(write_value, 0, MAX_VALUE);
  analogWrite(A0, write_value);

  // write to the file
  logfile.print(millis());
  logfile.print("\t");
  logfile.println(write_value, log_precision);
  return;
}

float convert2voltage(int value) {
  return MAX_VOLTAGE * value / MAX_VALUE;
}
