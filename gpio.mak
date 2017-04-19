BUILDGPIO := $(CC) $(CFLAGS) -c -o opendevice.o opendevice.c

GPIO = opendevice.o -lusb
