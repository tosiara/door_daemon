door_daemon
===========

## About

A daemon for door sensor. It monitors gpio pin status and as soon as it changes - executes a script.

## Hardware

Requires GPIO and a sensor. Supported GPIO hardware:

* wiringPi - wiringPi library required (not included)
* USBasp - custom firmware required (included)
* LPT port - papapin library used to access LPT (included)

Any NC ("norlmally closed") type sensor can be used, for example magnetic door sensor


## Compile

Checkout a branch that is matching your GPIO hardware and compile. For example, to compile wiringPi based door daemon:
* git checkout gpio_wiringPi
* make

See README.md in the branch for hardware specific tips

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
