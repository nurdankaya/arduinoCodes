//send T to serial2 and take a temperature

#define RXD2 16
#define TXD2 17
char c;
String readString;
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  Serial2.write("T");
  delay(3000);
  Serial.print("temperature: ");
  takeValue();

  Serial2.write("H");
  delay(3000);
  Serial.print("Humidity: ");
  takeValue();
}

void takeValue(){
  while (Serial2.available()) {
    c = Serial2.read();
    readString += c;
  }
  if (readString.length() > 0) {
    Serial.print(readString);
    Serial2.print(readString);
    //server.print(readString);
    readString = "";
  }
  
}
