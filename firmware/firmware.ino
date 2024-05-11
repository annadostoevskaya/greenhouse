/*
 * Author: @github.com/annadostoevskaya
 * Filename: firmware.ino
 * Created: 12 May 2024 2:39:28 AM
 * Last Update: 12 May 2024 2:39:50 AM
 *
 * Description:
 * - Libraries:
 * - arduino-cli lib install Ethernet@2.0.2
 * -
 * - Hardware:
 * - Ethernet Shield <EMPTY>
 * - 
 */

#include <stdint.h>

#include <SPI.h>
#include <Ethernet.h>

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};


// TODO(annad): Parse this data from .ini file
uint8_t        g_MAC[] = { 0x6B, 0x62, 0x75, 0x5F, 0x67, 0x68 };
EthernetServer g_Server(80);

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
  if (Ethernet.begin(g_MAC))
  {
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
    Serial.println("Initializing server...");
    g_Server.begin();
    Serial.println("Start to listen for clients");
  }
  else
  {
    Serial.println("Failed to configure Ethernet using DHCP.");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)  Serial.println("Ethernet shield was not found.");
    else if (Ethernet.linkStatus() == LinkOFF)            Serial.println("Ethernet cable is not connected.");
    for (;;) { delay(1); }
  }
}

void request_handling(EthernetClient& cli)
{

}

void loop()
{
  EthernetClient cli = g_Server.available();

  if (cli)
  {
    request_handling(&cli);
  }

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
