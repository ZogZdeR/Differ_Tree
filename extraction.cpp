#include "extraction.hpp"

size_t file_length (FILE *stream) 
{
    struct stat statbuf;
    fstat (fileno(stream), &statbuf); // FIXME: ИСправить с теста на онегина
    return (size_t)statbuf.st_size;
}

int file_to_buffer (FILE *stream, char *buffer)
{
    size_t check = fread (buffer, sizeof (char), file_length (stream),  stream);
    if (check == file_length (stream)) 
    {
        buffer [file_length(stream)] = '\0';
        return 1;
    }
    return 0;
}