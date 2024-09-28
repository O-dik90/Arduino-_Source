//GPIO 34
const int pH_pin = 34; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  readPH();
}

void readPH() {
  //float pH_value = analogRead(pH_pin);
  float pH_value = random(200001,304599)/100.0;

  // 0-4095 -> input voltage from 0-3.3volt/0.5volt
  // 0-14 -> standart pH value
  float pH_volt = pH_value * (3.3/4095.0);
  float pH_res = pH_volt *3.3;

  Serial.print("analog_value: " + String(pH_value) + "\t");
  Serial.print("pH_volt: " + String(pH_volt) + "\t");
  Serial.println("pH: " + String(pH_res));
  delay(1000);
}
