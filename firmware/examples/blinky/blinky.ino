void setup() {
  pinMode(8, OUTPUT);
}

void loop() {
  digitalWrite(8, LOW); // Enable
  delay(1000);
  digitalWrite(8, HIGH); // Disable
  delay(1000);
}

