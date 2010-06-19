/*
 * The main file for interaction with our bot.
 */

#include "sock.h"

int
main(int argc, char **argv)
{
    char host[MAXBUF] = {0};
    char port[MAXBUF] = {0};

    if (argc != 2) {
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
