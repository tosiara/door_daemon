door_daemon
===========

## About

A daemon for door sensor. It monitors gpio pin status and as soon as it changes - executes a script.

## Hardware

This branch requires any hardware compatible with `WiringPi` (RaspberryPi, BananaPi, Odroid etc)

Any NC ("norlmally closed") type sensor can be used, for example magnetic door sensor


## Compile

```
git checkout gpio_wiringPi
make
```

## Use

Start daemon and specify command parameters which scripts to run:
```
./door-daemon -b /path/to/boot_event_script.sh -s /path/to/start_event_script.sh -e /path/to/end_event_script.sh -p PIN -o OUT
```
PIN is an integer number representing pin number.
OUT is output pin number. If not defined - will be ignored.
Default value is `0`, so don't forget to specify your real pin.

If none params specified, following scripts are executed:
```
Boot - /usr/bin/gpio-boot.sh
Event start - /usr/bin/gpio-start.sh
Event end - (none)
```
For debugging and verbose output use `-d` parameter. Dry run `-x` will keep reading pin, but take no actions
