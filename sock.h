#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXBUF 100
#define MSGBUF 100

#define nick "fagbot"

int handler(char *, char *, char *);
