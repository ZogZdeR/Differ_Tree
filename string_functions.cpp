#include "string_functions.hpp"

#include <ctype.h>
#include <stdlib.h>

void MyGetline (char *Source)
{
    assert (Source != NULL);

    int i = -1;
    do
    {
        i++;
        Source [i] = (char)getchar ();
    }
    while (Source[i] != EOF and Source[i] != '\n');
    Source [i] = '\0';
}

void MyPuts (const char *Arr)
{
    assert (Arr != NULL);

    for (int i = 0; Arr[i] != '\0'; i++)
    {
        putchar (Arr[i]);
    }
    putchar ('\n');
}


const char * MyStrchr (const char *Arr, char c)
{
    assert (Arr != NULL);
    assert (c != '\0');

    int i = 0;
    while (Arr[i] != '\0')
    {
        if (Arr[i] == c) return &(Arr[i]);
        i++;
    }
    return NULL;
}

int MyStrlen (const char *Arr)
{
    assert (Arr != NULL);
    int size = 0;
    while ((Arr[size] != '\0'))
    {
        size++;
    }
    return size;
}

void MyStrcpy (const char *Original, char *Parody)
{
    assert (Original != NULL);
    assert (Parody != NULL);

    size_t i = 0;
    do
    {
        Parody[i] = Original[i];
        i++;
    }
    while (Original[i] != '\0');
    Parody [i] = '\0';
}

char *MyStrcat (char *destination, const char *to_paste)
{
    assert (destination != NULL);
    assert (to_paste != NULL);

    int l = MyStrlen (destination);  // номер эллемента массива, с когорого начнется to_paste
    int i = 0;
    while (i < MyStrlen (to_paste))
    {
        destination[i + l] = to_paste[i];
        i++;
    }
    destination[i + l] = '\0';
    return destination;
}

int MyStrcmp (const char *First, const char *Second)
{
    assert (First != NULL);
    assert (Second != NULL);

    size_t m = min (MyStrlen (First), MyStrlen (Second));
    size_t i = 0; 
    while (i < m)
    {
        if (First[i] > Second[i]) return 1;
        else if (First[i] < Second[i]) return -1;
        i++;
    }
    if (First[i] == '\0' && Second[i] != '\0') return -1;
    else if (Second[i] == '\0' && First[i] != '\0') return 1;
    else return 0;
}

int MyStrncmp (const char *First, const char *Second, size_t max_n)
{
    assert (First != NULL);
    assert (Second != NULL);

    size_t m = min (MyStrlen (First), MyStrlen (Second));
    size_t i = 0; 
    while (i < m && i < max_n)
    {
        if (First[i] > Second[i]) return 1;
        else if (First[i] < Second[i]) return -1;
        i++;
    }
    if (i == max_n) return 0;
    if (First[i] == '\0' and Second[i] != '\0') return -1;
    else if (Second[i] == '\0' && First[i] != '\0') return 1;
    else return 0;
}

char *MyStrdup (const char *Orig)
{
    assert (Orig != NULL);

    char *Copy = (char*) calloc((size_t)(MyStrlen(Orig) + 1), sizeof(char));
    assert (Copy != NULL);
    MyStrcpy (Orig, Copy);
    return Copy; 
}

size_t min (int a, int b)
{
    if (a < b) return (size_t)a;
    else return (size_t)b;
}

void MyStrncpy (const char *Original, char *Parody, int n)
{
    assert (Original != NULL);
    assert (Parody != NULL);
    assert (n >= 0);

    int i = 0;
    while (Original[i] != '\0' && i < n)
    {
        Parody[i] = Original[i];
        i++;
    }
    Parody [i] = '\0';    
}

char *MyStrncat (char *destination, const char *to_paste, int n)
{
    assert (destination != NULL);
    assert (to_paste != NULL);

    int l = MyStrlen (destination);  // номер эллемента массива, с когорого начнется to_paste
    int i = 0;
    while (i < MyStrlen (to_paste) && i < n)
    {
        destination[i + l] = to_paste[i];
        i++;
    }
    destination [i + l] = '\0';
    return destination;
}

int MyAtoi (const char *Source)
{
    assert (Source != NULL);

    int resoult = 0;
    int len = MyStrlen(Source);
    int deg = 1;
    for (int i = len - 1; i >= 0; i--)
    {
       resoult += deg * (Source[i] - '0');
       deg *= 10; 
    }
    return resoult;
}



char *MyFgets (char *input, int maxn, FILE *stream)
{
    assert (stream != NULL);

    int i = 0;
    input[i] = '\0';
    while (i < maxn && input[i] != EOF)
    {   
        input[i] = (char)fgetc (stream);
        if (input[i] != '\n' && input[i] != EOF) i++;
        else  if (input[i] == EOF)
        {
            input[i] = '\0';
            break;
        }
        else input [i] = '\0';
    }
    input[i] = '\0';
    return input;
}