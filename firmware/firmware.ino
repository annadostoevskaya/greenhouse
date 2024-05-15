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
 * - IniFile@1.3.0
 * -
 */

#include <stdint.h>

#include <IniFile.h>
#include <SPI.h>
#include <SD.h>

enum EthernetDHCPStatus
{
  ETHERNET_DHCP_NOTHING = 0,
  ETHERNET_DHCP_RENEW_FAILED,
  ETHERNET_DHCP_RENEW_SUCCESS,
  ETHERNET_DHCP_REBIND_FAILED,
  ETHERNET_DHCP_REBIND_SUCCESS
};


void printErrorMessage(uint8_t e, bool eol = true)
{
  switch (e) {
  case IniFile::errorNoError:
    Serial.print("no error");
    break;
  case IniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case IniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case IniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case IniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case IniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case IniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case IniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Info: Intializing SD card...");
  if (!SD.begin(4))
  {
    // TODO(annad): Error handling!
    Serial.println("Error: SD card initialization failed!");
    return;
  }

  if (!SD.exists("CONFIG.INI"))
  {
    // TODO(annad): Error handling!
    Serial.println("Error: Can't find config file!");
    return;
  }

  IniFile ini("CONFIG.INI");

  if (!ini.open()) {
    Serial.print("Ini file does not exist");
    // Cannot do anything else
    while (1)
      ;
  }
  Serial.println("Ini file exists");

  const size_t size = 80;
  char buffer[size];
  for (size_t i = 0; i < size; i += 1)
  {
    buffer[i] = 0;
  }

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, size)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    printErrorMessage(ini.getError());
    while (1)
      ;
  }
  
  // Fetch a value from a key which is present
  if (ini.getValue("network", "mac", buffer, size)) {
    Serial.print("section 'network' has an entry 'mac' with value ");
    Serial.println(buffer);
  } else {
    Serial.print("Could not read 'mac' from section 'network', error was ");
    printErrorMessage(ini.getError());
  }

/*
  File f_Config = SD.open("CONFIG.INI");

  Serial.println("Info: Allocating memory for config file...");
  size_t cfg_size = f_Config.size();
  char *cfg_content = (char *)malloc(f_Config.size()) + 1;
  cfg_content[cfg_size] = '\0';
  if (!cfg_content)
  {
    Serial.println("Error: Failed to allocate memory!");
    return;
  }

  Serial.println("Info: Reading config file...");
  if (f_Config.read(cfg_content, cfg_size) == -1)
  {
    Serial.println("Error: Failed to read config file.");
  }

  Serial.println("Config content: ");
  Serial.println(cfg_content);

  f_Config.close();
*/
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
