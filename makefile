
CC = clang
LIBS = -lcurl

get-ip: main.c
	$(CC) -std=c89 -Wall -o get-ip main.c $(LIBS)

install: get-ip
	cp ./get-ip /usr/local/bin
