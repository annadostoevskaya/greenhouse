/*
 * Author: @github.com/annadostoevskaya
 * Filename: firmware.ino
 * Created: 12 May 2024 2:39:28 AM
 * Last Update: 12 May 2024 2:39:50 AM
 *
 * Description:
 * - Hardware:
 * - Ethernet Shield <EMPTY>
 * -
 * - Used platform:
 * - arduino:avr@1.8.6
 * -
 * - Libraries:
 * - SPI@1.0
 * - Ethernet@2.0.2
 * - SD@1.2.4
 * - "DHT sensor library"@1.4.6
 * - "Adafruit TSL2561"@1.1.2
 */

#include <stdint.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <SPI.h>
#include <DHT.h>
#include <Ethernet.h>
#include <SD.h>

#define _DEBUG

#include "ini.h"
#include "NoCString.h"

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 0x752);

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  /**************************** Basic IO setup ***************************/
  setup_speaker();
  Serial.begin(9600);

  /**************************** Setup tsl2561 ****************************/
  if (!tsl.begin())
  {
    Serial.println(F("Error: No TSL2561 detected... Check your wiring or I2C ADDR!"));
    for (;;) { delay(10); }
  }

  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);

  /**************************** Setup dht11 ******************************/
  dht.begin();

  /**************************** Setup SD card ****************************/
  Serial.println(F("Info: Intializing SD card..."));
  if (!SD.begin(4))
  {
    Serial.println(F("Error: SD card initialization failed!"));
    for (;;) { delay(10); }
  }

  /**************************** Reading config ***************************/
  if (!SD.exists("CONFIG.INI"))
  {
    Serial.println(F("Error: Can't find config file!"));
    for (;;) { speaker(500); }
  }

  File f_Config = SD.open("CONFIG.INI");
  Serial.println(F("Info: Allocating memory for config file..."));
  size_t cfg_size = f_Config.size();
  char *cfg_content = (char *)malloc(f_Config.size()) + 1;
  cfg_content[cfg_size] = '\0';
  if (!cfg_content)
  {
    Serial.println(F("Error: Failed to allocate memory!"));
    for (;;) { speaker(500); }
  }

  Serial.println(F("Info: Reading config file..."));
  if (f_Config.read(cfg_content, cfg_size) == -1)
  {
    Serial.println(F("Error: Failed to read config file."));
    for (;;) { speaker(500); }
  }

  f_Config.close();
  Serial.println(cfg_content);

  NoCString network_mac       = ini_get_value(cfg_content, "network", "MAC");
  NoCString api_check_access  = ini_get_value(cfg_content, "api",     "check_access");
  NoCString api_update_state  = ini_get_value(cfg_content, "api",     "update_state");
  NoCString http_auth         = ini_get_value(cfg_content, "http",    "authorization");
  NoCString proxy_auth        = ini_get_value(cfg_content, "proxy",   "authorization");

  /**************************** Setup Ethernet ***************************/
  Serial.println(F("Info: Intializing Ethernet..."));
  uint8_t g_MAC[6] = parse_mac(network_mac); // { 0x00, 0x12, 0x034, 0x56, 0x78, 0x90 };
  if (Ethernet.begin(g_MAC) == 0)
  {
    Serial.println(F("Error: Failed to configure Ethernet using DHCP."));
    if (Ethernet.hardwareStatus() == EthernetNoHardware)  Serial.println(F("Error: Ethernet shield was not found."));
    else if (Ethernet.linkStatus() == LinkOFF)            Serial.println(F("Error: Ethernet cable is not connected."));
    for (;;) { delay(10); }
  }

  Serial.print(F("Info: IP address: "));
  Serial.println(Ethernet.localIP());
}

void loop()
{
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light)
  {
    Serial.print(F("L: "));
    Serial.print(event.light);
    Serial.print(F("lux"));
  }
  else
  {
    Serial.println("Error: Sensor overload");
    for (;;) { delay(10); }
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Error: Failed to read from DHT sensor!"));
    for (;;) { delay(10); }
  }

  Serial.print(F(" H: "));
  Serial.print(h);
  Serial.print(F("% T: "));
  Serial.print(t);
  Serial.println(F("°C"));

  delay(250);

  EthernetClient eth0;
  HttpClient http = HttpClient(eth0, "google.com", 80);
  http.get("/");

  uint32_t status = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(status);
  Serial.println("Response: ");
  Serial.println(response);
  delay(10000);

  switch (Ethernet.maintain())
  {
    case ETHERNET_DHCP_RENEW_SUCCESS:
    {
      Serial.println(F("Info: Renewed success"));
      Serial.print(F("Info: My IP address: "));
      Serial.println(Ethernet.localIP());
    } break;

    case ETHERNET_DHCP_REBIND_SUCCESS:
    {
      Serial.println(F("Info: Rebind success"));
      Serial.print(F("Info: My IP address: "));
      Serial.println(Ethernet.localIP());
    } break;

    case ETHERNET_DHCP_RENEW_FAILED: Serial.println(F("Error: renewed fail")); break;
    case ETHERNET_DHCP_REBIND_FAILED: Serial.println(F("Error: rebind fail")); break;
    default: {} break;
  }
}
