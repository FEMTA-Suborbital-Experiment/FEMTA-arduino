// the setup function runs once when you press reset or power the board

int OPEN_PIN = 13;
int CLOSE_PIN = 12;
int count = 0;

void setup() {
  // initialize digital pin 13 as an output.
  pinMode(OPEN_PIN, OUTPUT);
  pinMode(CLOSE_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
   open();
   delay(1000);
   close();
   delay(1000);
   count++;

   if(count == 10){
    return;
   }
}

void open(){
  digitalWrite(OPEN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(CLOSE_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(OPEN_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
}

void close() {
  digitalWrite(CLOSE_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(OPEN_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(CLOSE_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
}
