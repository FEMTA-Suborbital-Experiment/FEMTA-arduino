int closeSV1{A1};
int openSV1{5};
int closeSV2{A2};
int openSV2{6};
int closeLV1{A3};
int openLV1{9};
int signal{13};

void setup() {
    Serial.begin(9600);

    pinMode(closeSV1, OUTPUT);
    pinMode(closeSV2, OUTPUT);
    pinMode(openSV1, OUTPUT);
    pinMode(openSV2, OUTPUT);
    pinMode(closeLV1, OUTPUT);
    pinMode(openLV1, OUTPUT);
    pinMode(signal, OUTPUT);
}

void loop() {
    digitalWrite(openSV2, LOW);
    digitalWrite(openSV1, LOW);
    digitalWrite(openLV1, LOW);
    digitalWrite(closeSV2, HIGH);
    digitalWrite(closeSV1, HIGH);
    digitalWrite(closeLV1, HIGH);
    Serial.println(digitalRead(openSV1));
    Serial.println(digitalRead(closeSV1));
    Serial.println(digitalRead(openSV2));
    Serial.println(digitalRead(closeSV2));
    Serial.println(digitalRead(openLV1));
    Serial.println(digitalRead(closeLV1));
    delay(1000);
    digitalWrite(openSV2, HIGH);
    digitalWrite(openSV1, HIGH);
    digitalWrite(openLV1, HIGH);
    digitalWrite(closeSV2, LOW);
    digitalWrite(closeSV1, LOW);
    digitalWrite(closeLV1, LOW);
    Serial.println(digitalRead(openSV1));
    Serial.println(digitalRead(closeSV1));
    Serial.println(digitalRead(openSV2));
    Serial.println(digitalRead(closeSV2));
    Serial.println(digitalRead(openLV1));
    Serial.println(digitalRead(closeLV1));
    delay(1000);
}