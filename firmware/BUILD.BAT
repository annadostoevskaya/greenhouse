@ECHO OFF

arduino-cli --no-color compile -u -t -p COM4 --verbose --warnings all -b arduino:avr:uno --output-dir build

