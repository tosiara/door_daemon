wiringPi-door-daemon
===========

## About

A daemon for door sensor

## Hardware

Uses GPIO header compatible with wiringPi library

## Compile

* cd client-src; make

## Use

### Door sensor

Edit functions eventDetected(), insideEvent() and eventEnded() to implement your event handler
