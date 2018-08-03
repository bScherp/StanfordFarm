//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
#define FIREBASE_HOST "stanford-farm.firebaseio.com"
#define FIREBASE_AUTH "QcFhTkA1yECwRqv2vjhWFOrZAzoIPcElAHQPOUdj"

//-----SETTINGS-----//
int postingInterval = 1000; // 5 seconds

// Wi-Fi Settings
//const char* ssid = "Stanford"; // your wireless network name (SSID)
//const char* password = ""; // your Wi-Fi network password
const char* ssid = "ahosler"; // your wireless network name (SSID)
const char* password = "somethingsexy"; // your Wi-Fi network password
WiFiClient client;

//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  connectToInternet();
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
  readAndUploadData();
  delay(postingInterval);
}

//-----SETUP METHODS-----//
void connectToInternet() {
  Serial.print("Connecting");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WIFI Connected");
  Serial.println();
}
  int num = 0;
  int digit = 0;
//-----HELPER METHODS-----//
void readAndUploadData() {
  if (Serial.available() > 0) {
    num = 0;
    while (Serial.available() > 0) {
       digit = Serial.read();
       if (digit != 13 && digit != 10) {
          num *= 10;
          num += digit-48;
       } 
    }
    Serial.println(num);
    Firebase.setInt("number", num);
    if (Firebase.failed()) {
        Serial.print("Firebase failure: ");
        Serial.println(Firebase.error());  
    }
  }  
  delay(1000);
  /*
  //int g1int = Firebase.getInt("Live/G1/sensor1");
  Firebase.setInt("S1", 100);
  Serial.print("G1 Integer: SET"); //Serial.println(g1int);
  if (Firebase.failed()) {
      Serial.print("Firebase failure: ");
      Serial.println(Firebase.error());  
  }
  
  delay(1000);
  Serial.println();
  */
//  Firebase.setInt("Live/G1/sensor1", 69);
//  Firebase.setInt(("Live/G2"), adc1);
//  Firebase.setInt(("Live/G3"), adc1);
//  Firebase.setInt(("Live/G4"), adc1);
//  Firebase.setInt(("Live/G5"), adc1);
//  Firebase.setInt(("Live/G6"), adc1);
//
//  String path = "Database/G1/sensor1";
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject& upload = jsonBuffer.createObject();
//  JsonObject& uploadTime = upload.createNestedObject("timestamp");
//  upload["value"] = adc0;
//  uploadTime[".sv"] = "timestamp";
//  Firebase.push(path, upload);
}
