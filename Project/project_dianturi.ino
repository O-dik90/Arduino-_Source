/*
 *  Created on: 18.01.2024
 *  Modified on: 11.05.2024
 *  Author : ODIK Y N
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <Servo.h>
#include "DHT.h"

#define USE_SERIAL Serial
#define DHTPIN 13
#define DHTTYPE DHT11
// #define ONE_WIRE_BUS 13
#define SERVO_PIN 4
#define pinRelay 15


LiquidCrystal_I2C lcd(0x27, 20, 4);
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
//DeviceAddress sensor;

WiFiMulti wifiMulti;

const char* ssid = "TENDY LIA";  //TENDY LIA //Home
const char* pass = "sayapsuci";  // //Home_123
String base = "https://cyclic-rest-iot.onrender.com";

// declare time zone
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200;  // jakarta GMT +7
const int daylightOffset_sec = 0;

char printBuffer[50];
char timeWeekDay[30];

const size_t bufferSize = JSON_OBJECT_SIZE(512);
DynamicJsonDocument doc(bufferSize);

int httpCode;
float h, t;
String payload, jsonString, timeMessage;

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

static const uint32_t delay_S = 1000UL;
static const uint32_t delay_M = delay_S * 60UL;

void setup() {
  Serial.begin(115200);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  lcd.init();
  lcd.backlight();

  dht.begin();
  //sensors.begin();
  //Serial.print("Ditemukan ");
  //Serial.print(sensors.getDeviceCount(), DEC);

  //sensors.setResolution(sensor, 12);

  // Serial.print("Parasite power : ");
  // if (sensors.isParasitePowerMode()) Serial.println("ON");
  // else Serial.println("OFF");

  // if (!sensors.getAddress(sensor, 0)) Serial.println("Tidak dapat menemukan sensor DS18B20");

  servo.attach(SERVO_PIN);
  pinMode(pinRelay, OUTPUT);

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1UL * delay_S);
  }

  wifiMulti.addAP(ssid, pass);

  Serial.println("Initializing");
  lcd.print("INITIALIZING...");
  lcd.setCursor(0, 2);
  lcd.print("PLEASE_WAITING____");
}

void loop() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    printLocalTime();

    if (timeMessage != "failed" || timeMessage == "success") {
      // -> sensor method
      sensorTemp();
      // -> method for get/post/put api
      String timeMinute = String(timeWeekDay).substring(17, 20);
      lcd.setCursor(0, 2);
      lcd.print("TRY_GET_NEW_DATA___");

      if (timeMinute.toInt() % 20 == 0) {
        postTermo();

        delay(1UL * delay_M);
        //lcd.clear();
      }
      getRelay();
    }
    //delay 5 s
    delay(3UL * delay_S);
  }
}

// local Time
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    timeMessage = "failed";
    return;
  }
  strftime(timeWeekDay, 30, "%d-%B-%Y %H:%M", &timeinfo);

  timeMessage = "success";

  lcd.setCursor(0, 0);
  lcd.print(&timeinfo, "%m/%d/%Y %H:%M");
}

// request API
bool requestHTTP(String url, String method, String data) {
  HTTPClient http;

  http.begin(url);
  http.addHeader("content-type", "application/json");

  if (method == "GET") {
    //get data json;
    httpCode = http.GET();
  } else if (method == "POST") {
    //post data json;
    httpCode = http.POST(data);
  } else if (method == "PUT") {
    //put data json
    httpCode = http.PUT(data);
  } else {
    //method not found
    Serial.println("method not found");
    return false;
  }

  Serial.println("[HTTP] " + method + "..." + String(url));

  if (httpCode > 0) {
    Serial.println("[HTTP] " + method + " code: " + String(httpCode));

    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();

      lcd.setCursor(0, 2);
      lcd.print("SUCCESS_" + method + "_____");
      //Serial.println(payload);
      return true;
    }
  } else {
    Serial.println("httpCode: " + String(httpCode));
    Serial.println("[HTTP] " + method);
    Serial.printf("Failed, error: %s\n ", http.errorToString(httpCode).c_str());

    lcd.setCursor(0, 2);
    lcd.print("ERROR_" + method + "_DATA____");
  }
  http.end();
  return false;
}

// sensor or motor
void runServo(int rotating) {
  Serial.println("Servo/Relay start");
  lcd.setCursor(0, 3);
  lcd.print("RELAY_ON__");

  //rel-> on
  digitalWrite(pinRelay, HIGH);
  //run servo
  for (int servoLoop = 0; servoLoop <= rotating; servoLoop++) {
    servo.write(0);
    delay(3UL * delay_S);
    servo.write(180);
    delay(3UL * delay_S);
  }
  // delay and motor rel off
  delay(2UL * delay_S);
  digitalWrite(pinRelay, LOW);
}

void sensorTemp() {
  // sensors.requestTemperatures();
  // float suhu = sensors.getTempC(sensor);

  // lcd.setCursor(0, 1);
  // lcd.print("Temp C: ");
  // lcd.print(suhu, 3);
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.println("Temp:" + String(t) + "C Hum:" + String(h) + "% \n");

  lcd.setCursor(0, 1);
  lcd.print("T:" + String(t) + String((char)223) + "C H:" + String(h) + "%");
}

void getRelay() {
  printLocalTime();
  StaticJsonDocument<bufferSize> jsonData;
  jsonString = "";
  payload = "";

  requestHTTP(base + "/rel-get/6603410169bdae33c35a3247", "GET", "");
  DeserializationError error = deserializeJson(doc, payload);

  // Check for parsing errors
  if (error) {
    Serial.print(F("Failed to parse JSON: "));
    Serial.println(error.c_str());

    lcd.setCursor(0, 3);
    lcd.print("Fail_Read_Data_");
    delay(3UL * delay_S);
    lcd.clear();
    return;
  }

  Serial.println("api :" + payload);

  String amount = doc["amount"];
  String desc = doc["description"];
  bool val = doc["sync"];
  bool status = doc["status"];
  String time1String = doc["time_1"];
  String time2String = doc["time_2"];
  String time3String = doc["time_3"];
  String load1 = doc["load_1"];
  String load2 = doc["load_2"];
  String load3 = doc["load_3"];
  String load4 = doc["load_4"];

  String timeCurrent = String(timeWeekDay).substring(14, 19);

  Serial.println("json parse description: " + desc + "\t val:" + String(val) + "\n");
  Serial.println("time api:" + time1String.substring(16, 21));
  Serial.println("time local:" + timeCurrent);

  if (status == false) {
    lcd.setCursor(0, 3);
    lcd.print("SYSTEM_SHUTDOWN_");
    return;
  } else if (val == true) {
    //sync now
    runServo(load4.toInt());

    jsonData["sync"] = false;
    jsonData["description"] = "Relay Off " + String(timeCurrent);
    serializeJson(jsonData, jsonString);

    lcd.setCursor(0, 2);
    lcd.print("UPDATE_DATA_RELAY_");
    lcd.setCursor(0, 3);
    lcd.print("RELAY_OFF______");

    requestHTTP(base + "/rel-update/6603410169bdae33c35a3247", "PUT", jsonString);
  } else if (time1String.substring(16, 21) == timeCurrent) {
    //time 1 -> load 1
    runServo(load1.toInt());
  } else if (time2String.substring(16, 21) == timeCurrent) {
    //time 2 -> load 2
    runServo(load2.toInt());
  } else if (time3String.substring(16, 21) == timeCurrent) {
    //time 3 -> load 3
    runServo(load3.toInt());
  } else {
    lcd.setCursor(0, 3);
    lcd.print("RELAY_OFF______");
  }
}

void postTermo() {
  StaticJsonDocument<bufferSize> jsonData;
  jsonString = "";
  payload = "";

  if (!isnan(h) || !isnan(t)) {
    jsonData["description"] = "new data termo " + String(timeWeekDay);
    jsonData["hum"] = String(h);
    jsonData["temp"] = String(t);
    jsonData["ph"] = "0.0";
    jsonData["hum_unit"] = "-";
    jsonData["temp_unit"] = "C";
    jsonData["ph_unit"] = "ph";

    serializeJson(jsonData, jsonString);
    Serial.println("data : " + jsonString);

    Serial.println("Post Temperature");
    lcd.setCursor(0, 2);
    lcd.print("POST_DATA_TEMP_____");

    //testing pakai update, setelah ready pakai POST method
    //requestHTTP(base + "/temp-update/66034d86616a56b2879fd0f2", "PUT", jsonString);

    requestHTTP(base + "/temp-add", "POST", jsonString);
    Serial.println("payload: " + String(payload) + "\n");
  }
}