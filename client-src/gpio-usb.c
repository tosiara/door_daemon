/* Name: gpio-usb 
 * Project page: http://homes-smart.ru/index.php/oborudovanie/uroki-i-primery/41-usb-kontroller-umnogo-doma
 * Last changes: 12.12.2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

/* common code moved to separate module */
#include "opendevice.h"
/* custom request numbers */
#include "requests.h"
#include "usbconfig.h"

usb_dev_handle      *handle = NULL;
char                buffer[8];
int                 cnt;

// 1,2,3,21,24,25

/* mode control without writing to EEROM */
int pinmode(int pin,int mode)
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		2, mode * 256 + pin, 0, buffer, sizeof(buffer), 5000);

	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		return -2;
	}

	return 1;
}

/* mode control with write to EEROM */
int pinmodeEEROM(int pin,int mode)
{
	cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		3, mode * 256 + pin, 0, buffer, sizeof(buffer), 5000);

	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
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
			return -1;
		}
		else
		{
			fprintf(stderr, "only %d bytes received.\n", cnt);
			return -1;
		}
	}

	return (buffer[0]);
}

/* port write */
int digitalwrite(int pin,int data)
{
	if (pin > 0 && pin < 9)
	{
		// CUSTOM_RQ_SET_STATUS
		cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
			1, data * 256 + pin, 0, buffer, sizeof(buffer), 5000);

		if(cnt < 0)
		{
			fprintf(stderr, "USB error: %s\n", usb_strerror());
			return -2;
		}

		return 1;
	}

	return -1;
}

static void usage(char *name)
{
    fprintf(stderr, "Example from http://homes-smart.ru/\n");
    fprintf(stderr, "Version gpio-usb 1.0 date 10.09.13\n");
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s on <GPIO number>..Port on\n", name);
    fprintf(stderr, "  %s off <GPIO number>.Port off\n", name);
    fprintf(stderr, "  %s mode .............Manage port mode\n", name);
    fprintf(stderr, "  %s status .......... Show status of GPIO output\n", name);
    fprintf(stderr, "  %s statusin ... .....Show status of GPIO input\n", name);
    fprintf(stderr, "  %s pwm3 <level>......Manage PWM on 3 GPIO\n", name);
    fprintf(stderr, "  %s pwm4 <level>......Manage PWM on 4 GPIO\n", name);
    fprintf(stderr, "  %s rcsend <key>......Send RCremote key\n", name);
    fprintf(stderr, "  %s dhtread...........Read sensor DHT11 or DHT22\n", name);
    fprintf(stderr, "  %s dhtsetup..........Check status and on/off DHT sensors\n", name);
}

int main(int argc, char **argv)
{
	//usb_dev_handle      *handle = NULL;
	const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
	char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
	//char                buffer[8];

	int                  vid, pid, isOn;

	usb_init();
	/* we need at least one argument */
	if(argc < 2)
	{
		usage(argv[0]);
		exit(1);
	}

	/* compute VID/PID from usbconfig.h so that there is a central source of information */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	/* The following function is in opendevice.c: */
	if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
		exit(1);
	}

	if(strcasecmp(argv[1], "dhtread") == 0)
	{
		cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			22, 0, 0, buffer, sizeof(buffer), 5000);

		if(cnt < 1)
		{
			if(cnt < 0) 
				fprintf(stderr, "USB error: %s\n", usb_strerror());
			else
				fprintf(stderr, "only %d bytes received.\n", cnt);
		}
		else
		{
			if (buffer[0]==0 && buffer[1]==0 && buffer[2]==0 && buffer[3]==0) printf("DHT was not found or was not set up using dhtsetup.\n");
			else if ((((unsigned char)buffer[0] + (unsigned char)buffer[1] +(unsigned char) buffer[2] + (unsigned char)buffer[3] )& 0xFF) == (unsigned char)buffer[4])
			{
				// dht11
				if (buffer[1]==0 && buffer[3]==0) printf("DHT11:%d %% %d C\n",(buffer[0]),(buffer[2]));
				else
				{
					// dht22
					float f, h;
					h = (unsigned char)buffer[0] * 256 + (unsigned char)buffer[1];
					h /= 10;

					f = ((unsigned char)buffer[2] & 0x7F)* 256 + (unsigned char)buffer[3];
					f /= 10.0;
					if ((unsigned char)buffer[2] & 0x80)  f *= -1;
					printf("DHT22: %.1f °C  %.1f %%\n", f, h);
				}
			}
			else
				printf("DHT error\n");
		}
	}
	else if(strcasecmp(argv[1], "status") == 0)
	{
		cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_GET_STATUS,
			0, 0, buffer, sizeof(buffer), 5000);

		if(cnt < 1)
		{
			if(cnt < 0)
				fprintf(stderr, "USB error: %s\n", usb_strerror());
			else
				fprintf(stderr, "only %d bytes received.\n", cnt);
		}
		else
		{
			// printf("LED is %s\n", buffer[0] ? "on" : "off");
			int f;
			for(f = 0; f < 8; f++)
				printf("LED %d is %s\n", f + 1, buffer[f] ? "on" : "off");
		}
	}
	// port status output in "input" mode
	else if(strcasecmp(argv[1], "statusin") == 0)
	{
		if(argc > 2)
		{
			int pin = atoi(argv[2]);
			printf("MODE PIN %d is %d\n",pin, digitalread(pin));
		}
		else
		{
			printf("all pins:\n");
			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				25, 0, 0, buffer, sizeof(buffer), 5000);

			if(cnt < 1)
			{
				if(cnt < 0)
					fprintf(stderr, "USB error: %s\n", usb_strerror());
				else
					fprintf(stderr, "only %d bytes received.\n", cnt);
			}
			else
			{
				int f;
				for(f = 0; f < 8; f++)
					printf("LED %d is %d\n", f + 1, (int)(buffer[f]));
			}
		}
	}
	else if(strcasecmp(argv[1], "mode") == 0)
	{
		// output of ports' modes
		if(argc < 4)
		{
			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				21, 0, 0, buffer, sizeof(buffer), 5000);

			if(cnt < 1)
			{
				if(cnt < 0)
					fprintf(stderr, "USB error: %s\n", usb_strerror());
				else
					fprintf(stderr, "only %d bytes received.\n", cnt);
			}
			else
			{
				int f;
				for(f = 0; f < cnt; f++)
				{
					if ((unsigned char)(buffer[f]) != 1)
						printf("LED %d mode output (%d)\n", f + 1, (unsigned char)(buffer[f]));
					else
						printf("LED %d mode input (%d)\n", f + 1, (unsigned char)(buffer[f]));
				}

				fprintf(stderr, "Switch port to input or output:  %s <GPIO number> <0 or 1>  \n", argv[0]);
			}
		}
		else
		{
			// set mode of 1 port
			pinmodeEEROM (atoi(argv[2]), atoi(argv[3]));
		}
	}
	else if(strcasecmp(argv[1], "rcsend") == 0)
	{
		if(argc > 2)
		{
			unsigned long key= atoi(argv[2]);
			if (key<531441)
			{
				int per=3;
				// manual period
				if(argc > 3) per = atoi(argv[3]);
				unsigned int keyh = (unsigned long)key >> 16;
				cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
					4, key, keyh + 256 * per, buffer, sizeof(buffer), 5000);
			}
			else printf("The code can\'t be more than 531440\n");
		}
		else printf("The required param is missing: number 0 to 531440\n");
	}
	else if(strcasecmp(argv[1], "pwm3") == 0)
	{
		if(argc > 2)
		{
			int pin= atoi(argv[2]);

			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				23, pin, 0, buffer, sizeof(buffer), 5000);

			if(cnt < 0)
				fprintf(stderr, "USB error: %s\n", usb_strerror());
		}
		else
			printf("The required param is missing: number 0 to 255\n");
	}
	else if(strcasecmp(argv[1], "pwm4") == 0)
	{
		if(argc > 2)
		{
			int pin= atoi(argv[2]);

			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				24, pin, 0, buffer, sizeof(buffer), 5000);

	        if(cnt < 0)
				fprintf(stderr, "USB error: %s\n", usb_strerror());
		}
		else
			printf("The required param is missing: number 0 to 255\n");
	}
	else if(strcasecmp(argv[1], "dhtsetup") == 0)
	{
		// dht sensor on/off
		if(argc > 2)
		{
			int mode= atoi(argv[2]);

			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				6, mode, 0, buffer, sizeof(buffer), 5000);

			if (mode == 1)
				printf("Configuration: DHT sensor is on, data refreshed very minute.\n");
			else
				printf("Configuration: DHT sensor is off\n");
		}
		else
		{
			cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				5, 0, 0, buffer, sizeof(buffer), 5000);

			if (buffer[0]==1)
				printf("Status: DHT sensor is on (%u)\n",(unsigned char)buffer[0]);
			else
				printf("Status: DHT sensor is off (%u)\n",(unsigned char)buffer[0]);

			printf("Configuration: %s dhtsetup 1 or 0 to enable or disable sensor polling\n",argv[0]);
		}
	}
	else if(strcasecmp(argv[1], "analog") == 0)
	{
		// analog port
		cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			17, 0, 0, buffer, sizeof(buffer), 5000);

		if(cnt < 1)
		{
			if(cnt < 0)
				fprintf(stderr, "USB error: %s\n", usb_strerror());
			else
				fprintf(stderr, "only %d bytes received.\n", cnt);
		}
		else
			printf("analog:%u (%i %i)\n",((unsigned char)buffer[0]+(unsigned char)buffer[1]*256),(unsigned char)buffer[0],(unsigned char)buffer[1]);
	}
	else if((isOn = (strcasecmp(argv[1], "on") == 0)) || strcasecmp(argv[1], "off") == 0)
	{
		if(argc > 2)
		{
			if (atoi(argv[2]) > 0 && atoi(argv[2]) < 9)
				digitalwrite (atoi(argv[2]), isOn);
			else
				printf("Port range 1 to 8\n");
		}
		else printf("Configuration: %s on <GPIO number> or off to turn off\n", argv[0]);
	}
	else
	{
		usage(argv[0]);
		exit(1);
	}

	usb_close(handle);
	return 0;
}
