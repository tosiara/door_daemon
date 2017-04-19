#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "parapin.h"

#define DOOR_PIN 17

void gpio_init()
{
	if (pin_init_user(LPT1) < 0)
		exit(1);

	pin_input_mode(LP_DATA_PINS);
	pin_input_mode(LP_SWITCHABLE_PINS);
}

int gpio_read()
{
	return pin_is_set(LP_PIN[DOOR_PIN]);
}

