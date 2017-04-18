CFLAGS += -O2 -g -Wall

all:
	$(CC) $(CFLAGS) -O3 -c -o gpio.o gpio.c
	$(CC) $(CFLAGS) -O3 door_daemon.c -o door_daemon gpio.o -lwiringPi -lpthread

clean:
	rm -f door_daemon gpio.o

