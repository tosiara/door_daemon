#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include <wiringPi.h>

unsigned int DOOR_PIN = 0;

void gpio_init (unsigned int config_pin)
{
	syslog (LOG_NOTICE, "wiringPiSetup");
	wiringPiSetup();
	DOOR_PIN = config_pin;
	syslog (LOG_NOTICE, "Setting pin %d to INPUT", DOOR_PIN);
	pinMode (DOOR_PIN, INPUT);
}

int gpio_read()
{
	return digitalRead (DOOR_PIN);
}

int gpio_write(int state)
{

	return 0;
}

