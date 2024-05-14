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
 * - ArduinoHttpClient@0.6.0
 * - SD@1.2.4
 * - ArduinoJson@7.0.4
 * -
 */

#include <stdint.h>

#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};

// TODO(annad): Parse this data from .json file
// config.json
// [{
//  "MAC": "00:aa:bb:cc:de:ad",
//  "server_port": 80,
//  "gh_token": "gh_YnVrZXRvdl9ncmVlbmhvdXNl"
// }]
//
// TODO(annad): DHCP don't allocate IP without 0x00 prefix in MAC address, 
//   Research this later!
uint8_t        g_MAC[6];
char           g_Token[] = "gh_YnVrZXRvdl9ncmVlbmhvdXNl"; // X-Green-House-Token:

char config_json[] = "{\n"
"  \"MAC\":\"00:aa:bb:cc:de:ad\",\n"
"  \"gh_token\": \"gh_YnVrZXRvdl9ncmVlbmhvdXNl\",\n"
"  \"endpoint.check_access\":\"/device/check_access\",\n"
"  \"endpoint.update_state\":\"/greenhouse/create\"\n"
"}\n";


void mac_str2arr(String s, uint8_t mac[6])
{
  // FIXME(annad): Cringe...
  (void)(s);
  uint8_t tmp[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0xDA };
  for (int16_t i = 0; i < 6; i += 1)
  {
    mac[i] = tmp[i];
  }
}

void setup()
{
  Serial.begin(9600);

  JsonDocument config;
  DeserializationError err = deserializeJson(config, config_json);

  if (err)
  {
    Serial.print("JSON deserialize failed: ");
    Serial.println(err.f_str());
    // TODO(annad): Error handling!
    for (;;) { delay(1); }
  }

  Serial.println("Device Config");
  Serial.print("MAC: ");
  Serial.println(config["MAC"].as<String>());
  Serial.print("gh_token: ");
  Serial.println(config["gh_token"].as<const char*>());
  Serial.print("endpoint.check_access: ");
  Serial.println(config["endpoint.check_access"].as<const char*>());
  Serial.print("endpoint.update_state: ");
  Serial.println(config["endpoint.update_state"].as<const char*>());

  mac_str2arr(config["MAC"].as<String>(), g_MAC);
  for (int i = 0; i < 6; i += 1)
  {
    Serial.print(g_MAC[i], HEX);
    Serial.print(" ");
  }

  return;

  // NOTE(annad):
  // [!] Disable the SD card by switching pin 4 high
  // not using the SD card in this program, but if an SD card is left in the socket,
  // it may cause a problem with accessing the Ethernet chip, unless disabled
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

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
}

void loop()
{
  return;

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
}
