#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Replace with your WiFi credentials
const char* ssid = "TENDY LIA";
const char* password = "sayapsuci";

// HiveMQ Cloud MQTT Broker settings
const char* mqtt_server = "b35cc5f785624b83a0681563d0dbf259.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "user09";
const char* mqtt_password = "Admin_123";

// Unique device identifier
const char* deviceId = "esp32_001";  // Each ESP32 has a unique device ID
char mqtt_topic[50];

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  espClient.setInsecure();  // For SSL
  client.setServer(mqtt_server, mqtt_port);
  snprintf(mqtt_topic, 50, "esp32/%s/sensor", deviceId);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //create new data
  NewData();
}

void NewData() {
  StaticJsonDocument<250> jsonData;
  String jsonString;

  float sensorValue = random(2211,2799)/100.0; 
  jsonData["sensor"] = sensorValue;
  jsonData["description"] = "new data";
  jsonData["status"] = false;
  
  serializeJson(jsonData, jsonString);
  Serial.println(jsonString);

  client.publish(mqtt_topic, (char*)jsonString.c_str());
  delay(2000);
}
