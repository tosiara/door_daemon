#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "parapin.h"

unsigned int DOOR_PIN = 0;
unsigned int OUT_PIN = 0;

void gpio_init (unsigned int config_pin, unsigned int out_pin)
{
	if (pin_init_user(LPT1) < 0)
		exit(1);

	if (config_pin)
	{
		DOOR_PIN = LP_PIN[config_pin];
		pin_input_mode(DOOR_PIN);
	}

	if (out_pin)
	{
		OUT_PIN = LP_PIN[out_pin];
		pin_output_mode(OUT_PIN);
		clear_pin (OUT_PIN);
	}
}

int gpio_read()
{
	return pin_is_set (DOOR_PIN);
}

int gpio_write(int state)
{
	if (state)
		set_pin (OUT_PIN);
	else
		clear_pin (OUT_PIN);

	return 0;
}

