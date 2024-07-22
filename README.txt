# FGL-GH-24

NOTE:
- [!] In Chinese version of Ethernet Shield LAN contact contacting Arduino board,
    so before power your device, check and make sure that doesn't: https://imgur.com/a/3BRN1gR
- [!] About Shield/SD: The FAT format uses the familiar but limited naming convention,
    where a file or directory name can have up to eight letters, followed by a period and then the three-letter extension.
- [!] For access to serial monitor just type `stty -F /dev/ttyUSB0 raw 9600 && cat /dev/ttyUSB0` at terminal

References:
- Ethernet Shield tutorials: https://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/
- Ethernet Shield Problem: https://www.seanet.com/~karllunt/w5100_library.html
- Ethernet Shield Schematic: https://www.arduino.cc/en/uploads/Main/arduino-ethernet-shield-06-schematic.pdf
- W5100 Datasheet: https://www.sparkfun.com/datasheets/DevTools/Arduino/W5100_Datasheet_v1_1_6.pdf
- Understanding SPI: https://youtu.be/0nVNwozXsIc
- [!] Chinese Arduino CH340G drivers: https://www.wch-ic.com/products/CH340.html
- AVR-libc malloc: https://www.nongnu.org/avr-libc/user-manual/malloc.html
- AVR progmem: https://gcc.gnu.org/onlinedocs/gcc/AVR-Variable-Attributes.html#index-progmem-variable-attribute_002c-AVR
- How to connect 2 computers direct: https://faqpc.ru/soedinit-dva-kompyutera-po-seti/
