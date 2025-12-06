#ifndef EXTRACTION
#define EXTRACTION

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

size_t file_length (FILE *stream);
int file_to_buffer (FILE *stream, char *buffer);

#endif // EXTRACTION