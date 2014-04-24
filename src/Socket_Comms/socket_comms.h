#ifndef SOCKET_COMMS_SEEN
#define SOCKET_COMMS_SEEN

int open_socket(char *host, char *port);
void error(char *msg);
int send_data(int socket, char *s);
int recv_data(int socket);
struct sockaddr_in getSocket(int port, char *uri);

#endif
