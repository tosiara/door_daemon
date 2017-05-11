#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "opendevice.h"
#include "requests.h"
#include "usbconfig.h"

#define DEBUG_GPIO 0

usb_dev_handle      *handle = NULL;
char                buffer[8];
int cnt;

unsigned int DOOR_PIN = 0;

/* mode control without writing to EEROM */
int pinmode(int pin, int mode)
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		2, mode * 256 + pin, 0, buffer, sizeof(buffer), 5000);

	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		syslog (LOG_NOTICE, "USB error: %s", usb_strerror());
		return -2;
	}

	return 1;
}

/* mode control with write to EEROM */
int pinmodeEEROM(int pin, int mode)
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		3, mode * 256 + pin, 0, buffer, sizeof(buffer), 5000);

	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		syslog (LOG_NOTICE, "USB error: %s", usb_strerror());
		return -2;
	}

	return 1;
}

/* port status read */
int digitalread(int pin)
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		24, pin, 0, buffer, sizeof(buffer), 5000);

	if(cnt < 1)
	{
		if(cnt < 0)
		{
			fprintf(stderr, "USB error: %s\n", usb_strerror());
			syslog (LOG_NOTICE, "USB error: %s", usb_strerror());
			return -1;
		}
		else
		{
			fprintf(stderr, "only %d bytes received.\n", cnt);
			syslog (LOG_NOTICE, "only %d bytes received.", cnt);
			return -1;
		}
	}

	return (buffer[0]);
}

/* port write */
int digitalwrite(int pin, int data)
{
	if (pin > 0 && pin < 9)
	{
		// CUSTOM_RQ_SET_STATUS
		cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
			1, data * 256 + pin, 0, buffer, sizeof(buffer), 5000);

		if(cnt < 0)
		{
			fprintf(stderr, "USB error: %s\n", usb_strerror());
			syslog (LOG_NOTICE, "USB error: %s", usb_strerror());
			return -2;
		}

		return 1;
	}

	return -1;
}

const unsigned char rawVid[2], rawPid[2];
char vendor[] = {USB_CFG_VENDOR_NAME, 0};
char product[] = {USB_CFG_DEVICE_NAME, 0};

void gpio_init (unsigned int config_pin)
{
	int vid, pid;

	DOOR_PIN = config_pin;

	usb_init();

	/* compute VID/PID from usbconfig.h so that there is a central source of information */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	/* The following function is in opendevice.c: */
	if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
		syslog (LOG_NOTICE, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x", product, vid, pid);
		exit(1);
	}

	/* set gpio PIN to input mode, turn gpio PIN on */
	pinmodeEEROM (DOOR_PIN, 1);
	digitalwrite (DOOR_PIN, 1);

}

int gpio_read()
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			25, 0, 0, buffer, sizeof(buffer), 5000);

	if (cnt < 1)
		return 1;
	if (DEBUG_GPIO) printf ("Pin \"%d\" value: %d\n", DOOR_PIN, buffer[DOOR_PIN - 1]);

	/* buffer[6] contains gpio 7 value */
	return buffer[DOOR_PIN - 1];
}

