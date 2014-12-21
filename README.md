usbasp-gpio
===========

## About

This is fork of the usb-gpio project: http://www.homes-smart.ru/index.php/oborudovanie/uroki-i-primery/41-usb-kontroller-umnogo-doma

Convert your USBasp into USB GPIO:
* Door sensor (NC/NO)
* DS18B20 thermometer
* DHT11/DHT22 humidity sensor
* and other

## Hardware

You need [USBasp - USB programmer for Atmel AVR controllers](http://www.fischl.de/usbasp/), you can buy one at ebay or aliexpress for $2.

## Compile

Prerequisites:
* libusb-devel

Compile:
* make

## Firmware

To flash firmware you need another USBasp or Arduino. Binary firmware is located in `firmware-bin` directory (source code will be published soon)

## Use

### Door sensor

<img src="http://www.homes-smart.ru/images/gpioUSB.PNG">

* Connect door sensor to pins 6 (gpio 7) and 10 (gpio gnd)
* Swith port to input: `sudo ./gpio-usb mode 7 1`
* Enable port: `sudo ./gpio-usb on 7`
* Read port status: `sudo ./gpio-usb statusin`
```
all pins:
LED 1 is 1
LED 2 is 1
LED 3 is 0
LED 4 is 0
LED 5 is 0
LED 6 is 0
LED 7 is 1
LED 8 is 0
```
* Toggle sensor and re-read status: `sudo ./gpio-usb statusin`
```
all pins:
LED 1 is 1
LED 2 is 1
LED 3 is 0
LED 4 is 0
LED 5 is 0
LED 6 is 0
LED 7 is 0
LED 8 is 0
```
