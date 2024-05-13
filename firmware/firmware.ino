/*
 * Author: @github.com/annadostoevskaya
 * Filename: firmware.ino
 * Created: 12 May 2024 2:39:28 AM
 * Last Update: 12 May 2024 2:39:50 AM
 *
 * Description:
 * - Libraries:
 * - Ethernet@2.0.0
 * - ArduinoHttpClient@0.6.0
 * - SD@1.2.4
 * -
 * - Hardware:
 * - Ethernet Shield <EMPTY>
 * - 
 */

#include <stdint.h>
// #include <string.h>

#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>

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
uint8_t        g_MAC[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0xAD };
char           g_Token[] = "gh_YnVrZXRvdl9ncmVlbmhvdXNl"; // X-Green-House-Token:

void setup() {
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
