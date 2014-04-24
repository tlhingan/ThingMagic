#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "socket_comms.h"

struct sockaddr_in getSocket(int port, char *uri)
{
  struct sockaddr_in myConnection;
  memset(&myConnection, 0, sizeof(myConnection));
	 myConnection.sin_family = AF_INET;
	 myConnection.sin_port = htons(port);
	 myConnection.sin_addr.s_addr = inet_addr(uri);
	 return myConnection;
}


int send_data(int socket, char *s)
{
  int result = send(socket, s, strlen(s), 0);
  if (result == -1)
    fprintf(stderr, "%s: %s\n", "Error talking to the server", strerror(errno));
  
  return result;
}

int recv_data(int socket)
{
  char rec[256]; 
  
  int bytesRcvd = recv(socket, rec, 255, 0);
  rec[bytesRcvd] = '\0';
  printf("%d\n", bytesRcvd);
  while(bytesRcvd != -1)
  {
    printf("In the listen to me while loop\n");
      fprintf(stdout, "%s", rec);
      bytesRcvd = recv(socket, rec, 255, 0);
      rec[bytesRcvd] = '\0';
  }
  return 0;
}

int open_socket(char *host, char *port)
{
  struct addrinfo *res;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if(getaddrinfo(host, port, &hints, &res) == -1)
      error("Can't resolve the address");
  
  int d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(d_sock == -1)
    error("Can't open socket");
  
  int c = connect(d_sock, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);
  if (c == -1)
    error("Can't connect to socket");
  return d_sock;
}

void error(char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}
