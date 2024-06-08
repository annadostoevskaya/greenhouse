/*
 * Author: @github.com/annadostoevskaya
 * Filename: firmware.ino
 * Created: 12 May 2024 2:39:28 AM
 * Last Update: 12 May 2024 2:39:50 AM
 *
 * Description:
 * - Deivce MAC address: 00:b0:5a:85:6b:00
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
 * - ArduinoHttpClient@0.6.0
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

bool parse_mac(NoCString& s, uint8_t mac[6])
{
  if (s.end - s.begin != 17) return false;

  memset(mac, 0, 6);

  int8_t i = 0;
  for (const char *iter = s.begin; iter < s.end; iter += 1)
  {
    if (*iter == ':' || *iter == '-')
    {
      i += 1;
      if (i >= 6) break;
      continue;
    }


    mac[i] *= 16;
    if (isdigit(*iter))
    {
      mac[i] += (*iter - '0');
      continue;
    }

    if (isxdigit(*iter))
    {
      mac[i] += (toupper(*iter) - 55);
      continue;
    }

    memset(mac, 0, 6);
    return false;
  }

  return true;
}

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};

Adafruit_TSL2561_Unified g_TSL2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 0x752);
DHT g_DHT(0x7, DHT22);

void setup()
{
  /**************************** Basic IO setup ***************************/
  setup_speaker();
  Serial.begin(9600);

  /**************************** Setup tsl2561 ****************************/
  if (!g_TSL2561.begin())
  {
    Serial.println(F("Error: No TSL2561 detected... Check your wiring or I2C ADDR!"));
    for (;;) { delay(10); }
  }

  g_TSL2561.enableAutoRange(true);
  g_TSL2561.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);

  /**************************** Setup dht11 ******************************/
  g_DHT.begin();

  /**************************** Setup SD card ****************************/
  Serial.println(F("Info: Intializing SD card..."));
  if (!SD.begin(4))
  {
    Serial.println(F("Error: SD card initialization failed!"));
    for (;;) { delay(10); }
  }

  /**************************** Reading config ***************************/
  Serial.println(F("Info: Reading config file..."));
  if (!SD.exists("CONFIG.INI"))
  {
    Serial.println(F("Error: Can't find config file!"));
    for (;;) { speaker(500); }
  }

  File f_Config = SD.open("CONFIG.INI");
  size_t cfg_size = f_Config.size();
  char *cfg_content = (char *)malloc(f_Config.size()) + 1;
  cfg_content[cfg_size] = '\0';
  if (!cfg_content)
  {
    Serial.println(F("Error: Failed to allocate memory!"));
    for (;;) { speaker(500); }
  }

  if (f_Config.read(cfg_content, cfg_size) == -1)
  {
    Serial.println(F("Error: Failed to read config file."));
    for (;;) { speaker(500); }
  }

  f_Config.close();

  NoCString network_mac       = ini_get_value(cfg_content, "network", "MAC");
  NoCString api_check_access  = ini_get_value(cfg_content, "api",     "check_access");
  NoCString api_update_state  = ini_get_value(cfg_content, "api",     "update_state");
  NoCString http_auth         = ini_get_value(cfg_content, "http",    "authorization");
  NoCString proxy_auth        = ini_get_value(cfg_content, "proxy",   "authorization");

  /**************************** Setup Ethernet ***************************/
  Serial.println(F("Info: Intializing Ethernet..."));
  uint8_t mac[6];
  if (!parse_mac(network_mac, mac))
  {
    Serial.println(F("Error: Failed to parse MAC address"));
    for (;;) { speaker(500); }
  }

  for (int i = 0; i < 6; i += 1)
  {
    Serial.print(mac[i], HEX);
    if (i < 5) { Serial.print(':'); }
  }
  Serial.println();

  if (Ethernet.begin(mac) == 0)
  {
    Serial.println(F("Error: Failed to configure Ethernet using DHCP."));
    if (Ethernet.hardwareStatus() == EthernetNoHardware)  Serial.println(F("Error: Ethernet shield was not found."));
    else if (Ethernet.linkStatus() == LinkOFF)            Serial.println(F("Error: Ethernet cable is not connected."));
    for (;;) { delay(10); }
  }

  Serial.print(F("Info: IP address: "));
  Serial.println(Ethernet.localIP());
}

EthernetClient g_Client;

void loop()
{
  sensors_event_t event;
  g_TSL2561.getEvent(&event);

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

  float h = g_DHT.readHumidity();
  float t = g_DHT.readTemperature();

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
