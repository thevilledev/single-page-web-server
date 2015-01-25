#define debug_print(fmt, ...) \
        do { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                     __LINE__, __func__ ); } while (0)

#define debug_print_fmt(fmt, ...) \
        do { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                     __LINE__, __func__, __VA_ARGS__); } while (0)

#ifndef UTIL_H
#define UTIL_H

void print_help();
int parse_arguments(int argc, char** argv);
int read_file_to_mem();

#endif
