BUILDGPIO := $(CC) $(CFLAGS) -c parapin.c -o parapin.o; ar -cr libparapin.a parapin.o
GPIO = libparapin.a

