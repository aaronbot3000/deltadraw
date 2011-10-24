PROJ = send-serial
OBJS = send-serial

CC = g++
CFLAGS = -O2 -Wall -Wextra
LIBS = -lopencv_core -lopencv_highgui

all: $(OBJS)

send-serial: send-serial.cpp
	$(CC) $(LIBS) $(CFLAGS) $< -o $@
