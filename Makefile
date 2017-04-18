CFLAGS += -O2 -g -Wall

all:
	$(CC) $(CFLAGS) door_daemon.c -o gpio-wiringPi -lwiringPi -lpthread

clean:
	rm door_daemon

