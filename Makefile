CC = gcc

all:
	$(CC) -Wall -Wextra -ggdb3 -o bot main.c sock.c
