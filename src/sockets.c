/*
 * A single-page web server.
 * Threaded.
 * HTTP/1.0.
 * Simple as that.
 *
 * Module:  sockets
 *
 * Author:  Ville Törhönen <ville@torhonen.fi>
 * Date:    01-24-2015
 * License: MIT License
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "sockets.h"
#include "config.h"
#include "util.h"

/* Global configuration parameters from server module. */
extern unsigned short PORT;
extern int DEBUG;
extern int IPV6_ONLY;

/* Function for setting up the socket. */
int setup_socket(int* socket_desc, struct sockaddr_in6* server) {
    *socket_desc = socket(AF_INET6, SOCK_STREAM, 0);
    if (*socket_desc == -1) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }
    if(DEBUG) debug_print_fmt("Socket %d created.\n", *socket_desc);

    /* Clear out the sockaddr_in6 before doing anything.*/
    memset(&(*server), 0, sizeof(*server));
    (*server).sin6_family = AF_INET6;
    (*server).sin6_addr = in6addr_any;
    (*server).sin6_port = htons(PORT);

    /* Re-use the socket, if possible. */
    static int y = 1;
    if(setsockopt(*socket_desc, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) < 0) {
        perror("Failed to set SO_REUSEADDR to our socket");
        return EXIT_FAILURE;
    }
    /* Check if we only want to use IPv6. */
    if (IPV6_ONLY) {
        if(DEBUG) debug_print("IPv4 support disabled. Only IPv6 socket will be used.\n");
        if(setsockopt(*socket_desc, IPPROTO_IPV6, IPV6_V6ONLY, &y, sizeof(int)) < 0) {
            perror("Could not set IPV6_V6ONLY socket option");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}

/* Binds the socket to the sockaddr_in6 server instance. */
int bind_socket(int* socket_desc, struct sockaddr_in6* server) {
    if(bind(*socket_desc, (struct sockaddr *)&(*server), sizeof(*server)) < 0) {
        perror("Failed to bind to the socket");
        return EXIT_FAILURE;
    }
    if(DEBUG) debug_print_fmt("Bind succeeded on port %hu.\n", PORT);
    return EXIT_SUCCESS;
}

/* Listens to the given socket. */
int listen_socket(int* socket_desc) {
    if(listen(*socket_desc, _QUEUE_LEN) < 0) {
        perror("Failed to listen to the socket");
        return EXIT_FAILURE;
    }
    if(DEBUG) debug_print("Listening to the socket. Waiting for connections.\n");
    return EXIT_SUCCESS;
}
