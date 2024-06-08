#include <stdint.h>

#include <Wire.h>
#include <Ethernet.h>
#include <SD.h>

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

void setup()
{
  /**************************** Basic IO setup ***************************/
  setup_speaker();
  Serial.begin(9600);

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
  if (g_Client.connect({192, 168, 0, 11}, 80))
  {
    g_Client.println("GET / HTTP/1.0");
    g_Client.println();
  }
  else
  {
    Serial.println(F("Error: Connection with server failed!"));
    for (;;) { speaker(1000); }
  }

  delay(1000); // Wait until server response

  while (g_Client.available())
  {
    char c = g_Client.read();
    Serial.print(c);
  }
  Serial.println();

  if (!g_Client.connected())
  {
    g_Client.stop();
  }

  delay(5000);

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
