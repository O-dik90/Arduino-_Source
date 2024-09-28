//kita akan membuat sketch dengan sensor suhu digital DS18B20
// sensor ini bisa digunakan dengan Arduino maupun ESP32
// karena keterbatasan wokwi maka simulasi ini dilakukan di Arduino Uno
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>  //https://github.com/milesburton/Arduino-Temperature-Control-Library/tree/master

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress sensor;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Mencari sensor DS18B20...");
  sensors.begin();
  Serial.print("Ditemukan ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" perangkat.");

  // report parasite power requirements
  Serial.print("Parasite power : ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(sensor, 0)) Serial.println("Tidak dapat menemukan sensor DS18B20");

  //atur resolusi suhu DS18B20 dengan resolusi 9-12 bit
  sensors.setResolution(sensor, 12);  //kita gunakan 12 bit untuk mendapatkan presisi yang paling baik
}

void loop() {
  //Ambil data suhu
  sensors.requestTemperatures();  // Send the command to get temperatures
  float suhu = sensors.getTempC(sensor);
  Serial.print("Temp C: ");
  Serial.println(suhu, 3);  // 4 angka di belakang koma
  lcd.clear();
  lcd.print("Temp C: ");
  lcd.print(suhu, 3);
  delay(1000);
}
