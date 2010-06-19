/*
 * socket operations.
 */

#include "sock.h"

/* Get socketaddr (for ip4/ip6)
 */

static void *getinaddr(struct sockaddr *sa);
static int to_send(char *, int);

static void *
getinaddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
	return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int
handler(char *client, char *host, char *port)
{
    int sock, bytes;
    char buf[MAXBUF] = {0};
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN] = {0};

    if (host == NULL) {
	host = "irc.gnu.org";
    }
    
    if (port == NULL) {
	port = "6667"; /* Default port */
    }
    
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
	fprintf(stderr, "[-] getaddrinfo: %s\n", gai_strerror(rv));
	return 1;
    }

    /* loop through all the results, and connect to the first available */
    for (p = servinfo; p != NULL; p = p->ai_next) {
	    if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	        perror("socket");
	        continue;
	    }
	
	    if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
	        close(sock);
	        perror("connect");
	        continue;
	    }

	    break;

    }

	if (p == NULL) {
	    fprintf(stderr, "[-] %s failed to connect\n", client);
	    return 2;
	}

	inet_ntop(p->ai_family, getinaddr((struct sockaddr *) p->ai_addr), s, 
		  sizeof (s));
	printf("[+] %s connecting to %s", client, host);
	freeaddrinfo(servinfo);

	char sendbuf[MSGBUF] = {0};
	
	sprintf(sendbuf, "USER %s \"\" \"127.0.0.1\" :%s\r\n", nick, nick);
	to_send(sendbuf, sock);

	sprintf(sendbuf, "NICK %s\r\n", nick);
	to_send(sendbuf, sock);
	
	while ((bytes = recv(sock, buf, MAXBUF - 1, 0)) != -1) {
	    buf[bytes] = '\0';
	    bytes = 0;
	    printf("%s", buf);
	}
	
	close(sock);
	
	return 0;
}

static int
to_send(char *msg, int sock)
{
    int check;
    
    check = send(sock, msg, strlen(msg), 0);
    if (check == -1) {
	perror("[-] send");
	return -1;
    }
    
    return 0;
}
