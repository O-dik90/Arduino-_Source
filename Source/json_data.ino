/**
 * BasicHTTPClient.ino
 *
 *  Created on: 16.01.2023
 *  Modified on: 18.01.2023  
 */
#include <ArduinoJson.h>

const size_t bufferSize = JSON_OBJECT_SIZE(5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  String res = jsonSerial();
  Serial.println(res + "\n");

  jsonDeserial(res);
  delay(5000);
}

String jsonSerial() {
  StaticJsonDocument<250> jsonData;
  String jsonString;
  jsonData["description"] = "create data json";
  jsonData["value"] = false;
  jsonData["sensor"] = 25.6;

  serializeJson(jsonData, jsonString);

  //Serial.println(jsonString);
  return jsonString;
}

void jsonDeserial(String jsondata) {
  //const char* = "{\"description\":\"create data json\",\"value\":false,\"sensor\":25.6}";
  DynamicJsonDocument doc(bufferSize);

  DeserializationError error = deserializeJson(doc, jsondata);

  // Check for parsing errors
  if (error) {
    Serial.print(F("Failed to parse JSON: "));
    Serial.println(error.c_str());
    return;
  }
  bool value = doc["value"];
  float sensor = doc["sensor"];

  Serial.println("Deserialize: " + String(sensor));
}

