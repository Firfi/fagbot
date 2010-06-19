/*
* socket operations.
*/

#include "sock.h"

/* Get socketaddr (for ip4/ip6)
*/

static void *getinaddr(struct sockaddr *sa);
static int to_send(char *, int);
static void usermsg(char *, char *, char *, int);
static void nickmsg(char *, char *, int);
static void privmsg(const char *, char *, char *, int);
static void joinmsg(const char *, char *, int);
static int getl(char *);

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
    
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
	fprintf(stderr, "[-] getaddrinfo: %s\n", gai_strerror(rv));
	return 1;
    }
    
    /* loop through all the results, and connect to the first available */
    for (p = servinfo; p != NULL; p = p->ai_next) {
	if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
	    == -1) {
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
    
    char nick[MSGBUF] = {0};
    char channel[MSGBUF] = {0};
    char sendbuf[MSGBUF] = {0};
    char privmsg_buffer[MSGBUF] = {0};
    char passbuf[_SC_PASS_MAX] = {0};

    printf("\n[+] NICK: ");
    getl(nick);
    printf("[+] CHANNEL: ");
    getl(channel);

    usermsg(nick, nick, sendbuf, sock);
    nickmsg(nick, sendbuf, sock);
    
    while ((bytes = recv(sock, buf, MAXBUF - 1, 0)) != -1) {
	buf[bytes] = '\0';
	bytes = 0;
	printf("%s", buf);
    }
    
    usleep(100);
    privmsg("nickserv", getpass(passbuf), sendbuf, sock);
    joinmsg(channel, sendbuf, sock);
    
    while (getl(privmsg_buffer) != 0)
	privmsg(channel, privmsg_buffer, sendbuf, sock);
    close(sock);
    
    return 0;
}

static void
usermsg(char *nick, char *nickname, char *sendbuf, int sock)
{
    snprintf(sendbuf, sizeof (MAXBUF), "USER %s \"\" \"127.0.0.1\" :%s\r\n",
nick, nickname);
    to_send(sendbuf, sock);
    return;
}
    
static void
nickmsg(char *nick, char *sendbuf, int sock)
{
    snprintf(sendbuf, sizeof (MAXBUF), "NICK %s\r\n", nick);
    to_send(sendbuf, sock);
    return;
}

static void
privmsg(const char *send_to, char *pass, char *sendbuf, int sock)
{
    snprintf(sendbuf, sizeof (MAXBUF), "PRIVMSG %s: %s\r\n", send_to, pass);
    to_send(sendbuf, sock);
    return;
}

static void
joinmsg(const char *channel, char *sendbuf, int sock)
{
    sprintf(sendbuf, "JOIN %s\r\n", channel);
    to_send(sendbuf, sock);
    return;
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

static int
getl(char *s)
{
    int c, d;
    
    for (d = 0; d < MAX-1 && ((c = getchar()) != EOF) &&
c != '\n'; ++d)
s[d] = c;
    if (c == '\n')
s[d++] = c;
    s[d] = '\0';
    
    return d;
}
