#define RXD2 16
#define TXD2 17

char operation = 0;
char c;
String temperature, humidity;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);

}

void loop() {

  if (Serial.available() > 0)
  {
    operation = Serial.read();
  }
  if (operation == '1') {

    digitalWrite(2, HIGH);

    writeStream(":");

    temperature = takeTempature();
    writeStream(temperature);

    writeStream(":");

    humidity =  takeHumidity();
    writeStream(humidity);

    writeStream(":");

    digitalWrite(2, LOW);
  }
  operation = 0;
}


String takeTempature() {
  Serial2.write("T");
  delay(3000);

  String readString = "";
  while (Serial2.available() ) {
    c = Serial2.read();
    readString += c;
  }
  return readString;
}

String takeHumidity() {
  Serial2.write("H");
  delay(3000);

  String readString = "";
  while (Serial2.available() ) {
    c = Serial2.read();
    readString += c;
  }

  return readString;
}


void writeStream(String data) {
  String temp = String(data);
  byte charBuf[temp.length()];
  temp.getBytes(charBuf, temp.length() + 1);
  Serial.write(charBuf, temp.length());
  Serial.flush();

}
