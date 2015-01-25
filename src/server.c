/*
 * A single-page web server.
 * Threaded.
 * HTTP/1.0.
 * Simple as that.
 *
 * Module:  main
 *
 * Author:  Ville Törhönen <ville@torhonen.fi>
 * Date:    01-24-2015
 * License: MIT License
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "sockets.h"
#include "http.h"
#include "util.h"

/* Initialize our global configuration. */
char FILENAME[_FILENAME_BUFSIZE] = {0};
char DATA[_DATA_BUFSIZE] = {0};
unsigned short PORT = _DEFAULT_HTTP_PORT;
int DEBUG = 0;
int IPV6_ONLY = 0;

int main(int argc , char *argv[]) {

    /* Read the CLI arguments. */
    int ret = 0;
    ret = parse_arguments(argc, argv);
    if (ret == -1) return EXIT_SUCCESS;
    else if (ret == 1) return EXIT_FAILURE;

    /* Read file to memory. */
    ret = read_file_to_mem();
    if (ret) return EXIT_FAILURE;

    /* Setup our socket. */
    int socket_desc;
    struct sockaddr_in6 server;
    ret = setup_socket(&socket_desc, &server);
    if (ret) return EXIT_FAILURE;

    /* Bind to the socket. */
    ret = bind_socket(&socket_desc, &server);
    if (ret) return EXIT_FAILURE;

    /* Listen to the socket with a backlog of _QLEN connections. */
    ret = listen_socket(&socket_desc);
    if (ret) return EXIT_FAILURE;

    /* Lanch threads as new requests arrive. */
    struct sockaddr_in client;
    int c = sizeof(struct sockaddr_in);
    while(true) {
        int client_sock = accept(socket_desc, (struct sockaddr*)&client, &c);
        if(client_sock < 0) {
            if(DEBUG) debug_print("Failed to accept client connection. Will continue.\n");
            continue;
        }
        if(DEBUG) debug_print("Accepted a connection.\n");

        /* Create a new thread. */ 
        pthread_t new_thread;
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        /* If we can't create a thread, we can't serve. Exit on failure. */
        if(pthread_create(&new_thread, NULL, http_thread, (void*)new_sock) < 0) {
            perror("Failed to create a thread");
            return EXIT_FAILURE;
        }
        if(DEBUG) debug_print("Connection handled to an HTTP thread.\n");
    }

    return EXIT_SUCCESS;
}
