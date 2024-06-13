const uint8_t g_LED_pin = 8;

void setup_speaker()
{
  pinMode(g_LED_pin, OUTPUT);
  digitalWrite(g_LED_pin, HIGH); // Disable
}

void speaker(uint32_t interval)
{
  digitalWrite(g_LED_pin, LOW); // Enable
  delay(interval);
  digitalWrite(g_LED_pin, HIGH); // Disable
  delay(interval);
}

void on_speaker()
{
  digitalWrite(g_LED_pin, LOW);
}

void off_speaker()
{
  digitalWrite(g_LED_pin, HIGH);
}
