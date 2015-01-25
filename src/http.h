#ifndef HTTP_H
#define HTTP_H

void* http_thread(void *socket_desc);
void gen_http_headers(char* buf, int status, char* content_type);

#endif
