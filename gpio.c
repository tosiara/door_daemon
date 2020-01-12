#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "parapin.h"

unsigned int DOOR_PIN = 0;

void gpio_init (unsigned int config_pin)
{
	DOOR_PIN = config_pin;

	if (pin_init_user(LPT1) < 0)
		exit(1);

	pin_input_mode(LP_DATA_PINS);
	pin_input_mode(LP_SWITCHABLE_PINS);
}

int gpio_read()
{
	return pin_is_set(LP_PIN[DOOR_PIN]);
}

int gpio_write(int state)
{

	return 0;
}

