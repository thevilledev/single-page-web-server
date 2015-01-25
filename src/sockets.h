#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#ifndef SOCKETS_H
#define SOCKETS_H

int setup_socket(int* socket_desc, struct sockaddr_in6* server);
int bind_socket(int* socket_desc, struct sockaddr_in6* server);
int listen_socket(int* socket_desc);

#endif
