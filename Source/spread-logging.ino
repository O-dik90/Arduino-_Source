/*
 * BasicDataLogging ESP32-SpreadSheet
 *
 *  Created on: 02.10.2024
 *  Modified on: 02.10.2024
 *  Hardware : ESP32 WROOM

 *  Url SpreadSheet: https://docs.google.com/spreadsheets/d/159XRHodar_W3z4PR72P_QzdVqm66tBdFOMacWFtJJp4/edit?gid=0#gid=0 
 *  Deploy to me(...@email.com) -> for anyone
 *  Script get di -> extensions-> app script
 */


#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TENDY LIA";
const char* password = "sayapsuci";

const char* serverName = "https://script.google.com/macros/s/AKfycbyLeJgbC0ilrgH2KiBEBVGfJcoeXS3o6KrcZNDPobKdF29kD1q2Y71ewB-MvVpf1Hal/exec";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Replace with your sensor data
    float temperature = random(2211,2999)/100.00;
    float humidity = random(6611,8999)/100.00;

    String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    Serial.println(jsonData);
    
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(60000);  // Send data every 3 seconds
}
