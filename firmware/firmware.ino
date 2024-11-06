/*
 * Author: @github.com/annadostoevskaya
 * Filename: firmware.ino
 * Created: 12 May 2024 2:39:28 AM
 * Last Update: 12 May 2024 2:39:50 AM
 */

#include <stdint.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <DHT.h>
#include <Ethernet.h>
#include <SD.h>

#include "ini.h"
#include "NoCString.h"
#include "parsers.h"

Adafruit_TSL2561_Unified g_TSL2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 0x752);
DHT g_DHT = DHT(0x7, DHT22);
/** \brief \link g_Server \endlink up server on Arduino Uno board, listen port 80. */
EthernetServer g_Server = EthernetServer(80);

void setup()
{
  /**************************** Basic IO setup ***************************/
  setup_speaker();
  Serial.begin(9600);

  /**************************** Setup tsl2561 ****************************/
  if (!g_TSL2561.begin())
  {
    Serial.println(F("Error: No TSL2561 detected... Check your wiring or I2C ADDR!"));
    for (;;) { speaker(2000); }
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
    for (;;) { speaker(2000); }
  }

  /**************************** Reading config ***************************/
  Serial.println(F("Info: Reading config file..."));
  if (!SD.exists(F("CONFIG.INI")))
  {
    Serial.println(F("Error: Can't find config file!"));
    for (;;) { speaker(500); }
  }

  File f_Config = SD.open(F("CONFIG.INI"));
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

  Serial.println("CONFIG.INI:");
  Serial.println(cfg_content);

  f_Config.close();

  NoCString network_mac     = ini_get_value(cfg_content, "network", "MAC");
  NoCString network_ip      = ini_get_value(cfg_content, "network", "IP");
  NoCString network_dns     = ini_get_value(cfg_content, "network", "DNS");
  NoCString network_gateway = ini_get_value(cfg_content, "network", "GATEWAY");
  NoCString network_subnet  = ini_get_value(cfg_content, "network", "SUBNET");

  /**************************** Setup Ethernet ***************************/
  Serial.println(F("Info: Intializing Ethernet..."));

  uint8_t mac[6];
  uint8_t ip[4];
  uint8_t dns[4];
  uint8_t gateway[4];
  uint8_t subnet[4];

  if (parse_mac(network_mac, mac)
    && parse_ip(network_ip, ip)
    && parse_ip(network_dns, dns)
    && parse_ip(network_gateway, gateway)
    && parse_ip(network_subnet, subnet))
  {
    Ethernet.begin(mac, ip, dns, gateway, subnet);

    Serial.print(F("Info: MAC address: "));
    Ethernet.MACAddress(mac);
    for (int i = 0; i < 6; i++) {
      Serial.print(mac[i], HEX);
      if (i < 5) {
        Serial.print(':');
      }
    }
    Serial.println();

    Serial.print(F("Info: IP address: "));
    Serial.println(Ethernet.localIP());
    Serial.print(F("Info: gateway IP: "));
    Serial.println(Ethernet.gatewayIP());
    Serial.print(F("Info: subnet: "));
    Serial.println(Ethernet.subnetMask());
    Serial.print(F("Info: DNS: "));
    Serial.println(Ethernet.dnsServerIP());
  }
  else
  {
    Serial.println(F("Error: Failed to parse MAC or IP/DNS/GATEWAY/SUBNET address"));
    for (;;) { speaker(500); }
  }
}

void loop()
{
  EthernetClient cli = g_Server.available();

  if (cli)
  {
    off_speaker();
    Serial.println(F("Info: Client available, quantities scanning..."));
    sensors_event_t event;
    g_TSL2561.getEvent(&event);
    float h = g_DHT.readHumidity();
    float t = g_DHT.readTemperature();

    bool eof = true;

    while (cli.connected())
    {
      if (cli.available())
      {
        char c = cli.read();
        // Serial.print(c);

        if (c == '\n' && eof)
        {
          Serial.println(F("Info: Sending..."));
          cli.println(F("HTTP/1.1 200 OK"));
          cli.println(F("Content-Type: application/json"));
          cli.println(F("Connection: close"));
          cli.println();
          cli.print(F("{\"H\":"));
          cli.print(h);
          cli.print(F(",\"T\":"));
          cli.print(t);
          cli.print(F(",\"L\":"));
          cli.print(event.light);
          cli.print('}');
          break;
        }

        if (c == '\n')
        {
          eof = true;
        }
        else if (c != '\r')
        {
          eof = false;
        }
      }
    }

    cli.flush();
    on_speaker();
    cli.stop();
  }
}
