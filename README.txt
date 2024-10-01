# FGL-GH-24

NOTE:
- [!] In the Chinese version of the Ethernet Shield LAN, contact the Arduino board. Before powering your device, check and make sure that it doesn't.
- [!] About the Shield/SD: The FAT format uses a familiar but limited naming convention, where a file or directory name can have up to eight letters, followed by a period and then a three-letter extension.
- [!] To access the serial monitor, just type `stty -F /dev/ttyUSB0 raw 9600 && cat /dev/ttyUSB0` in the terminal.
- [!] For connection via a crossover cable: `sudo ip addr add 10.0.0.1/24 brd + dev enp0s25`

References:
- Ethernet Shield tutorials: https://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/
- Ethernet Shield Problems: https://www.seanet.com/~karllunt/w5100_library.html
- Ethernet Shield Schematic: https://www.arduino.cc/en/uploads/Main/arduino-ethernet-shield-06-schematic.pdf
- W5100 Datasheet: https://www.sparkfun.com/datasheets/DevTools/Arduino/W5100_Datasheet_v1_1_6.pdf
- Understanding SPI: https://youtu.be/0nVNwozXsIc
- [!] Chinese Arduino CH340G drivers: https://www.wch-ic.com/products/CH340.html
- AVR-libc malloc: https://www.nongnu.org/avr-libc/user-manual/malloc.html
- AVR progmem: https://gcc.gnu.org/onlinedocs/gcc/AVR-Variable-Attributes.html#index-progmem-variable-attribute_002c-AVR
- How to connect 2 computers directly: https://faqpc.ru/soedinit-dva-kompyutera-po-seti/
