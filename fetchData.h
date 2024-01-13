#ifndef FETCHDATA_H
#define FETCHDATA_H
#include <stddef.h>
size_t write_callback(void *contents, size_t size, size_t nmemb, char **output);
char* perform_http_request(const char *output_filename);
#endif
