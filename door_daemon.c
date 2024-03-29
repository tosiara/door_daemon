#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include "gpio.h"
#include "door_daemon.h"


int main(int argc, char *argv[])
{
	int i;
	char buff_script_start[255];
	char buff_script_end[255];

	for (i = 1; i < argc; i++)
	{
		if (strcmp ("-b", argv[i]) == 0 && (argc > i+1) && argv[i+1][0] != '-')
			script_boot = argv[i+1];
		if (strcmp ("-s", argv[i]) == 0 && (argc > i+1) && argv[i+1][0] != '-')
		{
			snprintf(buff_script_start, 255, "%s &", argv[i+1]);
			script_start = buff_script_start;
		}
		if (strcmp ("-e", argv[i]) == 0 && (argc > i+1) && argv[i+1][0] != '-')
		{
			snprintf(buff_script_end, 255, "%s &", argv[i+1]);
			script_end = buff_script_end;
		}
		if (strcmp ("-p", argv[i]) == 0 && (argc > i+1) && argv[i+1][0] != '-')
			pin_config = atoi (argv[i+1]);
		if (strcmp ("-o", argv[i]) == 0 && (argc > i+1) && argv[i+1][0] != '-')
			out_pin = atoi (argv[i+1]);
		if (strcmp ("-d", argv[i]) == 0)
			DEBUG = 1;
		if (strcmp ("-x", argv[i]) == 0)
			DRY_RUN = 1;

	}

	syslog (LOG_NOTICE, "Door daemon start");
	event_boot();

	gpio_init (pin_config, out_pin);

	int breakMarker = 0;

	/* initial state must be "0" - "door closed" */
	int lastState = GPIO_STATE_CLOSED;
	int lastOut = GPIO_OUT_LOW;

	while (!breakMarker)
	{
		int doorState = gpio_read();
		if (DEBUG)
		{
			printf ("digitalRead: %d\n", doorState);
			syslog (LOG_NOTICE, "digitalread %d", doorState);
		}

		/*  only 0 or 1 are possible
			if we read something different, like 2 or -1
			then  we assume it is non-zero (door open) */
		if (doorState != GPIO_STATE_CLOSED)
			doorState = GPIO_STATE_OPENED;

		int sensorToggled = (doorState != lastState);

		if (!DRY_RUN)
		{

			if (out_pin)
			{
				int ext;

				if (DEBUG)
				{
					printf ("readExternal %d...:\n", out_pin);
					syslog (LOG_NOTICE, "readExternal %d...", out_pin);
				}

				ext = readExternal();
				if (DEBUG)
				{
					printf ("ext=%d\n", ext);
					syslog (LOG_NOTICE, "ext=%d\n", ext);
				}

				if (ext >= 0 && ext <= 255 && ext != lastOut)
				{
					if (DEBUG)
						printf ("read external: %d\n", ext);
					if (!DRY_RUN)
						writeOutput (ext);
					lastOut = ext;
				}
			}

			if (sensorToggled && lastState == GPIO_STATE_CLOSED)
				breakMarker = eventDetected();
			else if (sensorToggled && lastState == GPIO_STATE_OPENED)
				breakMarker = eventEnded();
			else if (!sensorToggled && lastState == GPIO_STATE_OPENED)
				breakMarker = insideEvent();
		}

		lastState = doorState;
		usleep (GPIO_SLEEP);
	}

	return 0;
}

void event_boot()
{
	int exec_res = 0;

	if (strlen (script_boot))
		exec_res = system (script_boot);

	if (exec_res)
		syslog (LOG_NOTICE, "event_boot() system exec failed: %d", exec_res);
}

int eventDetected()
{
	int exec_res = 0;

	syslog (LOG_NOTICE, "Event start");
	if (DEBUG)
		printf ("Event start\n");
	
	if (strlen (script_start))
		exec_res = system (script_start);

	if (exec_res)
		syslog (LOG_NOTICE, "eventDetected() system exec failed: %d", exec_res);

	return 0;
}

int insideEvent()
{
	if (DEBUG)
		printf ("Event inside\n");

	return 0;
}

int eventEnded()
{
	int exec_res = 0;

	syslog (LOG_NOTICE, "Event end");
	if (DEBUG)
		printf ("Event end\n");

	if (strlen (script_end))
		exec_res = system (script_end);

	if (exec_res)
		syslog (LOG_NOTICE, "eventEnded() system exec failed: %d", exec_res);

	return 0;
}

int readExternal()
{
	char c;
	FILE *fin = fopen("/dev/shm/gpio", "rb");
	if (!fin)
		return 1024;

	if (fscanf (fin, "%c", &c) == EOF)
		return 1025;
	fclose (fin);

	return c;
}

int writeOutput(int val)
{
	return gpio_write (val);
}

