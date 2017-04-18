#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include <wiringPi.h>

#define DOOR_PIN 6

void gpio_init()
{
	syslog (LOG_NOTICE, "wiringPiSetup");
	wiringPiSetup();
	syslog (LOG_NOTICE, "Setting pin %d to INPUT", DOOR_PIN);
	pinMode (DOOR_PIN, INPUT);
}

int gpio_read()
{
	return digitalRead (DOOR_PIN);
}

