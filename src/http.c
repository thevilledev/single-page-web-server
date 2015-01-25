/*
 * A single-page web server.
 * Threaded.
 * HTTP/1.0.
 * Simple as that.
 *
 * Module:  http
 *
 * Author:  Ville Törhönen <ville@torhonen.fi>
 * Date:    01-24-2015
 * License: MIT License
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "config.h"
#include "util.h"
#include "http.h"

extern char DATA[_DATA_BUFSIZE];
extern char FILENAME[_FILENAME_BUFSIZE];
extern int DEBUG;

/* Here is our HTTP thread. */
void* http_thread(void* socket_desc) {
    int sock = *(int*)socket_desc;
    static int ret = 0;
    static int file_fd = 0;
    static int read_size = 0;
    static char client_data[_REQUEST_BUFSIZE + 1];
    
    /* Read data from client. */ 
    read_size = recv(sock, client_data, _REQUEST_BUFSIZE, 0);

    /* Check if we got anything. Negative value on error. */
    if(read_size <= 0) {
        if(DEBUG) debug_print("Failed to read data from client.\n");
        client_data[0] = 0;
    }
    /* If we got something, terminate the buffer. */
    else {
        client_data[read_size] = 0;
        /* Check that we are handling a GET request. Otherwise return HTTP 501 (Not Implemented). */
        if(strncmp(client_data, "GET ", 4)) {
            if(DEBUG) debug_print("Received a non-GET request. Returning HTTP 501.\n");
            gen_http_headers(client_data, 501, NULL);
            write(sock, client_data, strlen(client_data));
        }
        else {
            if(DEBUG) debug_print("Received a GET request.\n");
            gen_http_headers(client_data, 200, "text/html");
            write(sock, client_data, strlen(client_data));
            write(sock, DATA, strlen(DATA));
            if(DEBUG) debug_print("Response sent.\n");
        }
    }

    /* Close the socket and free the descriptor. */
    if(DEBUG) debug_print("Closing socket.\n");
    close(sock);
    free(socket_desc);
    /* Quit the thread. */
    pthread_exit(NULL);
}

/* Generates HTTP headers to the given buffer. */
void gen_http_headers(char* buf, int status, char* content_type) {
    switch (status) {
        case 200:
                    assert(content_type != NULL);
                    sprintf(buf, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);
                    break;
        case 204:
                    assert(content_type == NULL);
                    sprintf(buf, "HTTP/1.0 204 No Content\r\n\r\n", content_type);
                    break;
        case 501:
                    assert(content_type == NULL);
                    strcat(buf, "HTTP/1.0 501 Not Implemented\r\nAllow: GET\r\n\r\n");
                    break;
        default:
                    assert(content_type == NULL);
                    strcat(buf, "HTTP/1.0 500 Internal Server Error\r\n\r\n");
                    break;
    }
}
