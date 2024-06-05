make
avr-objcopy -O ihex -R .eeprom main.elf main.hex
avrdude -p m2560 -P /dev/ttyUSB0 -c -b 115200 -D -q -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf -c wiring -U flash:w:main.hex:i
