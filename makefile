CC=gcc
CFLAGS=-lwiringPi -lwiringPiDev -lpthread -I.
DEPS = lcd.h
OBJ = lcd.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lcd: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
