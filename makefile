all: lcd.out led.out

CC=gcc
CCFLAGS=-Wall -I.
DEPLOY_CCFLAGS=-lwiringPi -lwiringPiDev -lpthread
DEPS = 

ifeq ($(OS),Windows_NT)
				CCFLAGS += -D WIN32
				ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
								CCFLAGS += -D AMD64
				endif
				ifeq ($(PROCESSOR_ARCHITECTURE),x86)
								CCFLAGS += -D IA32
				endif
else
				UNAME_S := $(shell uname -s)
				ifeq ($(UNAME_S),Linux)
								CCFLAGS += -D LINUX
				endif
				ifeq ($(UNAME_S),Darwin)
								CCFLAGS += -D OSX
								OBJ += lcd_osx.o
				endif
				UNAME_P := $(shell uname -p)
				ifeq ($(UNAME_P),x86_64)
								CCFLAGS += -D AMD64
				endif
				ifneq ($(filter %86,$(UNAME_P)),)
								CCFLAGS += -D IA32
				endif
				ifneq ($(filter arm%,$(UNAME_P)),)
								CCFLAGS += -D ARM
								CCFLAGS += $(ARM_CCFLAGS)
								OBJ += lcd_odroid.o
				endif
endif


UNAME_P := $(shell uname -p)

lcd.out: lcd.c
	gcc -o $@ $^ $(CCFLAGS) $(DEPLOY_CCFLAGS)

led.out: led.c
	gcc -o $@ $^ $(CCFLAGS) $(DEPLOY_CCFLAGS)

.PHONY: clean
clean:
	-rm *.o

# END
