CFLAGS += -O3 -Wall
GPIO = 

include gpio.mak

all:
	$(BUILDGPIO)
	$(CC) $(CFLAGS) -c -o gpio.o gpio.c
	$(CC) $(CFLAGS) door_daemon.c -o door_daemon gpio.o -lpthread $(GPIO)

clean:
	rm -f door_daemon *.o

