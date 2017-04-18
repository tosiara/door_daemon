CFLAGS += -O3 -Wall

all:
	$(CC) $(CFLAGS) -c -o gpio.o gpio.c
	$(CC) $(CFLAGS) door_daemon.c -o door_daemon gpio.o -lpthread

clean:
	rm -f door_daemon gpio.o

