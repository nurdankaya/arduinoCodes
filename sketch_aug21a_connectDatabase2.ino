#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFi.h>

const char* ssid = "-----";
const char* password = "----------";

char user[] = "root"; // MySQL user login username
char passwordSQL[] = "passwPhp1."; // MySQL user login password

char INSERT_SQL[] = "INSERT INTO temperature_db.machine1(date_curr,time_curr,temperature,humidity) VALUES ('2020-08-21','58:50:13',26.13,50.45);";

IPAddress server_addr(192,168,43,118); // IP of the MySQL server here


WiFiServer  server(3306);
void setup() {
  //Código de configuração aqui
  Serial.begin(115200);

  connectToNetwork();
  mySqlLoop();
}

void loop() {
}
void connectToNetwork() {
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }
 
  Serial.println("Connected to network");
 
}
void mySqlLoop(){
  WiFiClient client;
  MySQL_Connection conn((Client *)&client);
  if (conn.connect(server_addr, 3306, user, passwordSQL)) {
    Serial.println("Database connected.");
  }
  else{
    Serial.println("Connection failed.");
    Serial.println("Recording data.");
  }
  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(INSERT_SQL);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
  Serial.println("closing connection\n");
  //client.stop();
}
