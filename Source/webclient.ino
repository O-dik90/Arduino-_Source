/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *  Modified on: 17.01.2023
 *  Hardware : ESP32 WROOM
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

int httpCode;

void setup() {
  USE_SERIAL.begin(115200);

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  wifiMulti.addAP("TENDY LIA", "sayapsuci");
  Serial.println("IP =" + String(WiFi.localIP()));
}

void loop() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    Serial.println("connected to network");

    requestHTTP("https://expressjs-postgres-production-cd3a.up.railway.app/api/relayget/1", "GET");
  
  delay(5000);
}

//urequest API
bool requestHTTP(const char* url, String method) {
  HTTPClient http;

  http.begin(url);
  http.addHeader("content-type", "application/json");
  
  if (method == "GET") {
    httpCode = http.GET();
  } else if (method == "POST") {
    //post data json;
    httpCode = http.POST("data");
  } else if (method == "PUT") {
    //put data json
    httpCode = http.PUT("data");
  } else {
    Serial.println("method not found");
    return false;
  }

  Serial.print("[HTTP] begin...\n");
  Serial.print("[HTTP] "+ method +"...\n" + String(url));

  if (httpCode > 0) {
    Serial.print("[HTTP] " + method);
    Serial.println(" code: %d\n" + String(httpCode));

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.println("httpCode: " + String(httpCode));
    Serial.println("[HTTP] "+ method);
    Serial.printf("Failed, error: %s\n ", http.errorToString(httpCode).c_str());
  }
  http.end();
  return true;
}
