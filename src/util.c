/*
 * A single-page web server.
 * Threaded.
 * HTTP/1.0.
 * Simple as that.
 *
 * Module:  util
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
#include <fcntl.h>

#include "config.h"
#include "util.h"

/* Global configuration parameters from server module. */
extern unsigned short PORT;
extern char FILENAME[_FILENAME_BUFSIZE];
extern char DATA[_DATA_BUFSIZE];
extern int DEBUG;
extern int IPV6_ONLY;

void print_help() {
    printf( "Single Page Web Server\n"
            "Options:\n"
            "   -p <port>: TCP port number, which will be listened to. Defaults to port 80.\n"
            "   -f <path>: Path to file, which will be served. Required.\n"
            "   -6:        Use only IPv6 sockets. By default both IPv4 and IPv6 sockets are used.\n"
            "   -d:        Enable debug logging.\n"
            "   -h:        Display help.\n"
    );
}

/* Command-line parser. */
int parse_arguments(int argc, char** argv) {
    static int fflag = 0;
    static int error = 0;
    static int index = 0;
    static int c = 0;
    static int tmp = 0;
    opterr = 0;
    while ((c = getopt (argc, argv, "f:p:d6h")) != -1) {
        switch (c) {
            case 'h':
                print_help();
                return -1;
            case '6':
                IPV6_ONLY = 1;
                break;
            case 'd':
                DEBUG = 1;
                break;
            case 'f':
                fflag = 1;
                strcat(FILENAME, optarg);
                break;
            case 'p':
                tmp = atoi(optarg);
                if(tmp <= 0 || tmp >= 65536) {
                    fprintf (stderr, "Port number '%s' is invalid.\n", optarg); error = 1;
                    break;
                }
                PORT = (unsigned short)tmp;
                break;
            case '?':
                if (optopt == 'f') {
                    fprintf (stderr, "Option -%c requires an argument. Use -h to print help.\n", optopt); error = 1;
                }
                else if (optopt == 'p') {
                    fprintf (stderr, "Option -%c requires an argument. Use -h to print help.\n", optopt); error = 1;
                }
                else if (isprint (optopt)) {
                    fprintf (stderr, "Unknown option `-%c'. Use -h to print help.\n", optopt); error = 1;
                }
                else {
                    fprintf (stderr, "Unknown option character `\\x%x'. Use -h to print help.\n", optopt); error = 1;
                }
            default:
                return EXIT_FAILURE;
        }
    }
    /* Check that we received the required parameters. */
    if (!fflag) {
        fprintf(stderr, "Missing -f parameter. Use -h to print help.\n");
        error = 1;
    }
    if (error) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}

/* Reads FILENAME to DATA. */
int read_file_to_mem() {
    static int fd = 0;
    if((fd = open(FILENAME, O_RDONLY)) == -1) {
        perror("Cannot read data file");
        return EXIT_FAILURE;
    }
    int rbytes = read(fd, DATA, _DATA_BUFSIZE - 1);
    return EXIT_SUCCESS;
}
