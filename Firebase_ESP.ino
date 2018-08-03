//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "stanford-farm.firebaseio.com/"
#define FIREBASE_AUTH "QcFhTkA1yECwRqv2vjhWFOrZAzoIPcElAHQPOUdj"
#define WIFI_SSID "Stanford"
#define WIFI_PASSWORD ""

String chipId = "Solenoids";

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  
  Serial.begin(9600);
  Serial.println("check");

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {
  String path = chipId + "/states/";
  FirebaseObject object = Firebase.get(path);
  
  int led1 = object.getInt("S1");
  //int led1 = Firebase.getInt("S1");
  Serial.print("Led1 is ");
  Serial.println(led1);
  int led2 = object.getInt("S2");
  Serial.print("Led2 is ");
  Serial.println(led2);


  // handle error
  if (Firebase.failed()) {
      Serial.print("setting failed:");
      Serial.println(Firebase.error());  
  }
  
  //digitalWrite(D1, led1);
  //digitalWrite(D2, led2);

  delay(1000);
}
