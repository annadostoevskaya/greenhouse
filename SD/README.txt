# CONFIG.INI

CONFIG.INI is a file stored on the Arduino Ethernet Shield SD card, which formatted as FAT16 and using 8.3 file naming convention.
So, you should use a file name with 8 letters in the main part and 3 in the extension, not counting the dot.

Also, delete comments from the file, because firmware.ino allocate block for it once.

;; This is section for configure network state of Ethernet Shield
[network]
MAC = 00:b0:5a:85:6b:00
;; [!] firmware.ino don't use SSL.
;; I stronly advise you to use a local IP address without access from the external network.
IP = 10.0.0.2

