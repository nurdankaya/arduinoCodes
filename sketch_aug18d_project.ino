
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#define RXD2 16
#define TXD2 17

const char* ssid     = "Androidn";
const char* password = "forme4Free.";

//mysql connection
char user[] = "-----"; // MySQL user login username
char passwordSQL[] = "----------"; // MySQL user login password
IPAddress server_addr(192, 168, 43, 118); // IP of the MySQL server here
WiFiServer  server(3306);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String databaseName = "temperature_db";
String tableName = "machine1";
String date_curr, time_curr;     //sql statement variables
String temperature, humidity;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

char c;
String readString;
String takeTempature();
void printDateAndLocalTime();

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");


  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(10800);  // ex: For UTC +1.00 : 3 * 60 * 60 : 10800   --> for offset=3


}

void loop() {
  printDateAndLocalTime();
  date_curr = dayStamp;
  time_curr = timeStamp;

  Serial.print("temperature: ");
  temperature = takeTempature().toFloat();
  Serial.println(temperature);

  Serial.print("humidity: ");
  humidity = takeHumidity().toFloat();
  Serial.println(humidity);
  String INSERT_SQL_TEMP = "INSERT INTO " + databaseName + "." + tableName + " (date_curr,time_curr,temperature,humidity) VALUES('" + date_curr + "','" + time_curr + "'," + temperature + "," + humidity + ");";
  Serial.println(INSERT_SQL_TEMP);


  // Length (with one extra character for the null terminator)         //convert string to char array
  int str_len = INSERT_SQL_TEMP.length() + 1;

  // Prepare the character array (the buffer)
  char INSERT_SQL[str_len];

  // Copy it over
  INSERT_SQL_TEMP.toCharArray(INSERT_SQL, str_len);
  mySqlLoop(INSERT_SQL);
}



void mySqlLoop(char insertSql[]) {

  WiFiClient client;
  MySQL_Connection conn((Client *)&client);
  if (conn.connect(server_addr, 3306, user, passwordSQL)) {
    Serial.println("Database connected.");
  }
  else {
    Serial.println("Connection failed.");
    Serial.println("Recording data.");
  }
  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(insertSql);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
  Serial.println("closing connection\n");
  //client.stop();
}



String takeTempature() {
  Serial2.write("T");
  delay(10000);

  readString = "";
  while (Serial2.available() ) {
    c = Serial2.read();
    readString += c;
  }

  return readString;
}

String takeHumidity() {
  Serial2.write("H");
  delay(10000);

  readString = "";
  while (Serial2.available() ) {
    c = Serial2.read();
    readString += c;
  }

  return readString;
}



void printDateAndLocalTime() {


 while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);


}
