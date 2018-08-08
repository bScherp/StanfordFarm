//-----INCLUDES-----//
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

//-----DEFINES-----//
//Solenoid I/O
#define BED1 16
#define BED2 5
#define BED3 4
#define BED4 0
#define BED5 2
#define BED6 14


//#define FIREBASE_HOST "stanford-farm.firebaseio.com"
//#define FIREBASE_AUTH "QcFhTkA1yECwRqv2vjhWFOrZAzoIPcElAHQPOUdj"
#define FIREBASE_HOST "stanford-smart-farm.firebaseio.com"
#define FIREBASE_AUTH "4JxR22sgq6LsuMhiTew2Qaq3Xh42JRukUvC46A5X"

//-----SETTINGS-----//
int postingInterval = 10000; // 10 seconds

// Irrigation
bool iG1 = false;
bool iG2 = false;
bool iG3 = false;
bool iG4 = false;
bool iG5 = false;
bool iG6 = false;

// Sensors
int g1_sensor1 = 0;
int g2_sensor1 = 0;
int g3_sensor1 = 0;
int g4_sensor1 = 0;
int g5_sensor1 = 0;
int g6_sensor1 = 0;


// Wi-Fi Settings
//const char* ssid = "Stanford"; // your wireless network name (SSID)
//const char* password = ""; // your Wi-Fi network password
const char* ssid = "ahosler"; // your wireless network name (SSID)
const char* password = "somethingsexy"; // your Wi-Fi network password
WiFiClient client;


//-----SETUP/LOOP-----//
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  connectToInternet();
  pinModeSetup();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
  readAndUploadData();
  processChanges();
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

void pinModeSetup() {
  // Init the irrigation Beds (if we want to do them through node)
  
  pinMode(BED1, OUTPUT);
  pinMode(BED2, OUTPUT);
  pinMode(BED3, OUTPUT);
  pinMode(BED4, OUTPUT);
  pinMode(BED5, OUTPUT);
  pinMode(BED6, OUTPUT);
  
  digitalWrite(BED1, LOW);
  digitalWrite(BED2, LOW);
  digitalWrite(BED3, LOW);
  digitalWrite(BED4, LOW);
  digitalWrite(BED5, LOW);
  digitalWrite(BED6, LOW);
}


//-----HELPER METHODS-----//
void readAndUploadData() {
  getSettings();
  getSensorReadings();
  getIrrigationFlags();
  setLiveFirebase();
  setDatabaseFirebase();
}

void processChanges() {
  // Initiate irrigation state
  digitalWrite(BED1, iG1);
  digitalWrite(BED2, iG2);
  digitalWrite(BED3, iG3);
  digitalWrite(BED4, iG4);
  digitalWrite(BED5, iG5);
  digitalWrite(BED6, iG6);
}

//-----FIREBASE METHODS-----//
void getSensorReadings() {
  String typeStr;
  int type;
  String num;
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
       typeStr = Serial.readStringUntil(':');
       type = typeStr.toInt();
       switch (type) {
          case 1:
            num = Serial.readStringUntil('#');
            g1_sensor1 = num.toInt();
            break;
          case 2:
            num = Serial.readStringUntil('#');
            g2_sensor1 = num.toInt();
            break;
          case 3:
            num = Serial.readStringUntil('#');
            g3_sensor1 = num.toInt();
            break;
          case 4:
            num = Serial.readStringUntil('#');
            g4_sensor1 = num.toInt();
            break;
          case 5:
            num = Serial.readStringUntil('#');
            g5_sensor1 = num.toInt();
            break;
          case 6:
            num = Serial.readStringUntil('#');
            g6_sensor1 = num.toInt();
            break;
          default:
            Serial.print("error: got the following string: ");
            Serial.println(type);
       }
       /*if (digit != 13 && digit != 10) {
          num *= 10;
          num += digit-48;
       } */
    }
  }
  Serial.println(g1_sensor1);
  Serial.println(g2_sensor1);
  Serial.println(g3_sensor1);
  Serial.println(g4_sensor1);
  Serial.println(g5_sensor1);
  Serial.println(g6_sensor1);
    /*Firebase.setInt("number", num);
    if (Firebase.failed()) {
        Serial.print("Firebase failure: ");
        Serial.println(Firebase.error());  
    } */
}

void getSettings() {
  int intervalSetting = Firebase.getInt("Settings/interval");
  postingInterval = intervalSetting * 1000;
  Serial.print("Sampling Rate: "); 
  Serial.print(intervalSetting); 
  Serial.println(" seconds");
}

void getIrrigationFlags() {
  iG1 = Firebase.getBool("iFlag/G1");
  iG2 = Firebase.getBool("iFlag/G2");
  iG3 = Firebase.getBool("iFlag/G3");
  iG4 = Firebase.getBool("iFlag/G4");
  iG5 = Firebase.getBool("iFlag/G5");
  iG6 = Firebase.getBool("iFlag/G6");
  
  Serial.print("iG1: "); Serial.println(iG1);
  Serial.print("iG2: "); Serial.println(iG2);
  Serial.print("iG3: "); Serial.println(iG3);
  Serial.print("iG4: "); Serial.println(iG4);
  Serial.print("iG5: "); Serial.println(iG5);
  Serial.print("iG6: "); Serial.println(iG6);
  Serial.println();
}

void setLiveFirebase() {
  Firebase.setInt("Live/G1/sensor1/value", g1_sensor1);
  Firebase.setInt("Live/G2/sensor1/value", g2_sensor1);
  Firebase.setInt("Live/G3/sensor1/value", g3_sensor1);
  Firebase.setInt("Live/G4/sensor1/value", g4_sensor1);
  Firebase.setInt("Live/G5/sensor1/value", g5_sensor1);
  Firebase.setInt("Live/G6/sensor1/value", g6_sensor1);
}

void setDatabaseFirebase() {
  firebaseFormattedDatabaseCall("Database/G1/sensor1", g1_sensor1);
  firebaseFormattedDatabaseCall("Database/G2/sensor1", g2_sensor1);
  firebaseFormattedDatabaseCall("Database/G3/sensor1", g3_sensor1);
  firebaseFormattedDatabaseCall("Database/G4/sensor1", g4_sensor1);
  firebaseFormattedDatabaseCall("Database/G5/sensor1", g5_sensor1);
  firebaseFormattedDatabaseCall("Database/G6/sensor1", g6_sensor1);
}

void firebaseFormattedDatabaseCall(String path, int adc) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& upload = jsonBuffer.createObject();
  JsonObject& uploadTime = upload.createNestedObject("timestamp");
  upload["value"] = adc;
  uploadTime[".sv"] = "timestamp";
  Firebase.push(path, upload);
}

