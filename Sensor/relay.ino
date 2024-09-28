#define relay1_pin 13
#define relay2_pin 12
#define relay3_pin 14
#define relay4_pin 27

int relay1_state = 0;
int relay2_state = 0;
int relay3_state = 0;
int relay4_state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relay1_pin, HIGH);
  delay(5000);
  digitalWrite(relay1_pin, LOW);
  delay(5000);

  digitalWrite(relay2_pin, HIGH);
  delay(5000);
  digitalWrite(relay2_pin, LOW);
  delay(5000);

  digitalWrite(relay3_pin, HIGH);
  delay(5000);
  digitalWrite(relay3_pin, LOW);
  delay(5000);

  digitalWrite(relay4_pin, HIGH);
  delay(5000);
  digitalWrite(relay4_pin, LOW);
  delay(5000);
}
