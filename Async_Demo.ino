/*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/


#include <EEPROM.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "MJ";
const char* password = "Manoj@1234";

AsyncWebServer server(80);

int address_ssid_String = 0;
int address_password_String = 101;

char receivedChar;
void write_ssid_data(void);
void write_password_data(void);
char broadcastAddress[100];  //= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char stored_password[50];
char stored_ssid[50];

void read_ssid(void);
void read_password(void);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.println("");
  EEPROM.begin(512);
  // Wait for connection
  delay(1000);
  Serial.print("ssid:");
  read_ssid();
  Serial.println("");
  Serial.print("password:");
  read_password();
  Serial.println("");
  //WiFi.begin(ssid, password);
  WiFi.begin(stored_ssid, stored_password);
  Serial.print("connecting");

  for (int i = 0; i < 15; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print("try");
      Serial.println(i);
      check_serial_data();
    }
  }
  Serial.println("");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hi ! WELOCME TO LOGICMO UFD SERVER.");
  });


  AsyncElegantOTA.begin(&server);  // Start AsyncElegantOTA
  server.begin();
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("HTTP server not connected ");
    Serial.println("");

  } else {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.println("HTTP server started");
  }
}

void loop(void) {
  check_serial_data();
}
void check_serial_data(void) {

  if (Serial.available()) {
    receivedChar = Serial.read();

    if (receivedChar == 'S') {
      write_ssid_data();
      receivedChar = '\0';
    } else if (receivedChar == 'P') {
      write_password_data();
      receivedChar = '\0';
    }
  }
}
void write_ssid_data(void) {

  int i = 0;
  while (Serial.available()) {

    char c = Serial.read();

    //receivedData += c;

    EEPROM.write(address_ssid_String + i, c);
    //Serial.print(c);
    i++;
  }
  EEPROM.write(address_ssid_String + (i), '\0');  //add nll char to end of array
  EEPROM.write(100, 1);                           //data present bit

  EEPROM.commit();
  Serial.print("EEPROM DATA WRITE:");
  for (int j = 0; j < 100; j++) {

    stored_ssid[j] = EEPROM.read(address_ssid_String + j);
    if (stored_ssid[j] == '\0') {
      break;  // Stop reading at null-terminator
    }
    Serial.print(stored_ssid[j]);
  }

  Serial.println("");
  if (EEPROM.read(100) == 1) {
    Serial.println("EEPROM WRITE: SUCESS");
  } else {

    Serial.println("EEPROM WRITE: FAIL");
  }
}
void write_password_data(void) {

  int i = 0;
  while (Serial.available()) {

    char c = Serial.read();

    //receivedData += c;

    EEPROM.write(address_password_String + i, c);
    // Serial.print(c, HEX);
    i++;
  }
  EEPROM.write(address_password_String + (i), '\0');  //add nll char to end of array
  EEPROM.write(200, 1);                               //data present bit

  EEPROM.commit();
  Serial.print("EEPROM DATA WRITE:");
  for (int j = 0; j < 100; j++) {
    stored_password[j] = EEPROM.read(address_password_String + j);
    if (stored_password[j] == '\0') {
      break;  // Stop reading at null-terminator
    }
    Serial.print(stored_password[j]);
  }

  Serial.println("");
  if (EEPROM.read(200) == 1) {
    Serial.println("EEPROM WRITE: SUCESS");
  } else {

    Serial.println("EEPROM WRITE: FAIL");
  }
}
void read_ssid(void) {

  for (int j = 0; j < 50; j++) {
    stored_ssid[j] = EEPROM.read(address_ssid_String + j);
    if (stored_ssid[j] == '\0') {
      break;  // Stop reading at null-terminator
    }
    Serial.print(stored_ssid[j]);
  }
}
void read_password(void) {

  for (int j = 0; j < 50; j++) {
    stored_password[j] = EEPROM.read(address_password_String + j);
    if (stored_password[j] == '\0') {
      break;  // Stop reading at null-terminator
    }
    Serial.print(stored_password[j]);
  }
}