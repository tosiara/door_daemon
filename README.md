parapin-door-daemon
===========

## About

A daemon for door sensor

## Hardware

Uses LPT port and parapin library

## Compile

First compile parapin library

* cd parapin-1.5.0; make
* cd client-src; make

## Use

### Door sensor

Edit functions eventDetected(), insideEvent() and eventEnded() to implement your event handler
