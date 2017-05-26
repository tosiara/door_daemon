door_daemon
===========

## About

A daemon for door sensor. It monitors gpio pin status and as soon as it changes - executes a script.

## Hardware

This branch requires `USBasp` with custom firmware (included)

Pinout:

![Usbasp gpio pinout](https://img-fotki.yandex.ru/get/196258/73339514.41/0_115d8a_5bf7013d_orig.png)

Connect your sensor to GND and any available pin, for example, 7 (inner "sys" number 6)
Any NC ("norlmally closed") type sensor can be used, for example magnetic door sensor

## Compile

* git checkout gpio_usbasp
* make

## Flash the firmware

Compile the firmware:
```
cd firmware
make hex
cd ..
```
You need another one USBasp to flash the firmware.
Plug the programmer-usbasp into a USB port, on the target-usbasp shorten JP2 jumper (self programming mode) and connect both usbasps using the special 8-pin cable.

Flash the firmware:
``
avrdude -c usbasp -p m8 -U flash:w:firmware/main.hex
``

## Use

Start daemon and specify command parameters which scripts to run:
```
./door-daemon -b /path/to/boot_event_script.sh -s /path/to/start_event_script.sh -e /path/to/end_event_script.sh -p PIN
```
PIN is an integer number representing pin number. Default value is `0`, so don't forget to specify your real pin

If none params specified, following scripts are executed:
```
Boot - /usr/bin/gpio-boot.sh
Event start - /usr/bin/gpio-start.sh
Event end - (none)
```
For debugging and verbose output use `-d` parameter. Dry run `-x` will keep reading pin, but take no actions
