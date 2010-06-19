/*
 * The main file for interaction with our bot.
 */

#include "sock.h"

static void usage(char *);

int
main(int argc, char **argv)
{
    char host[MAXBUF] = {0};
    char port[MAXBUF] = {0};

    if (argc == 2) {
	usage(argv[0]);
	exit(-1);
    }
    
    if (argc != 3) {
	sprintf(host, "irc.gnu.org");
	sprintf(port, "6667");
	handler(argv[0], host, port);
    } else {
	if (strlen(argv[1]) < MAXBUF && strlen(argv[2]) < MAXBUF) {
	    strcpy(host, argv[1]);
	    strcpy(port, argv[2]);
	    handler(argv[0], host, port);
	} else {
	    fprintf(stderr, "[-] reduce your buffers!\n");
	    exit(-1);
	}
    }
    
    return 0;
}

static void
usage(char *argv)
{
    fprintf(stderr, "[-] fagbot v0.1\n");
    fprintf(stderr, "[-] usage: %s <host> <port> (or leave empty for irc.gnu.org\n", argv);

    return;
}
