const int trigger = 12;
const int echo = 14;

#define SOUND_SPEED 0.034

long duration;
float distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  dist();
  delay(1000);
}

void dist() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  
  // Calculate the distance
  distance = (duration * SOUND_SPEED/2)*10;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (mm): ");
  Serial.println(distance);
}