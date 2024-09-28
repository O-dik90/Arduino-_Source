#include <Servo.h>
#define SERVO_PIN 13
Servo servo;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  servo.attach(SERVO_PIN);
}

void loop() {
  servo.write(0);
  delay(5000);
  servo.write(180);
  delay(5000);
}
