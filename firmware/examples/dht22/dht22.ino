/*
 * Author: @github.com/annadostoevskaya
 * Filename: dht22.ino
 * Created: 18 May 2024 15:52:28 AM
 * Last Update: 18 May 2024 15:52:50 AM
 *
 * Description: <empty>
 */

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.println("°C");

  return;
}
