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
 * -
 */

#include <stdint.h>

#include <SPI.h>
#include <SD.h>

#define _DEBUG

#include "NoCString.h"
#include "ini.h"

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};

void setup()
{
  Serial.begin(9600);

  Serial.println(F("Info: Intializing SD card..."));
  if (!SD.begin(4))
  {
    // TODO(annad): Error handling!
    Serial.println(F("Error: SD card initialization failed!"));
    return;
  }

  if (!SD.exists("CONFIG.INI"))
  {
    // TODO(annad): Error handling!
    Serial.println(F("Error: Can't find config file!"));
    return;
  }

  File f_Config = SD.open("CONFIG.INI");

  Serial.println(F("Info: Allocating memory for config file..."));
  size_t cfg_size = f_Config.size();
  char *cfg_content = (char *)malloc(f_Config.size()) + 1;
  cfg_content[cfg_size] = '\0';
  if (!cfg_content)
  {
    Serial.println(F("Error: Failed to allocate memory!"));
    return;
  }

  Serial.println(F("Info: Reading config file..."));
  if (f_Config.read(cfg_content, cfg_size) == -1)
  {
    Serial.println(F("Error: Failed to read config file."));
  }

  f_Config.close();

  NoCString network_mac       = ini_get_value(cfg_content, "network", "MAC");
  NoCString api_check_access  = ini_get_value(cfg_content, "api",     "check_access");
  NoCString api_update_state  = ini_get_value(cfg_content, "api",     "update_state");
  NoCString http_auth         = ini_get_value(cfg_content, "http",    "authorization");
  NoCString proxy_auth        = ini_get_value(cfg_content, "proxy",   "authorization");

  network_mac.println();
  api_check_access.println();
  api_update_state.println();
  http_auth.println();
  proxy_auth.println();

  free(cfg_content);

  return;
/*
  Serial.begin(9600);

  while (!Serial) { ; }

  Serial.println("Intializing Ethernet...");
  if (Ethernet.begin(g_MAC) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP.");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)  Serial.println("Ethernet shield was not found.");
    else if (Ethernet.linkStatus() == LinkOFF)            Serial.println("Ethernet cable is not connected.");
    for (;;) { delay(1); }
  }

  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
*/
}

void loop()
{
  return;
/*
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
      Serial.println("Renewed success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
    } break;

    case ETHERNET_DHCP_REBIND_SUCCESS:
    {
      Serial.println("Rebind success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
    } break;

    case ETHERNET_DHCP_RENEW_FAILED: Serial.println("Error: renewed fail"); break;
    case ETHERNET_DHCP_REBIND_FAILED: Serial.println("Error: rebind fail"); break;
    default: {} break;
  }
*/
}
