#ifndef SOME_FUNCTION
#define SOME_FUNCTION

#include <stdio.h>
#include <math.h>
# include <assert.h>


void MyGetline (char *Source);
void MyPuts (const char *Arr);
const char * MyStrchr (const char *Arr, char c);
int MyStrlen (const char *Arr);
void MyStrcpy (const char *Original, char *Parody);
char *MyStrcat (char *destination, const char * to_paste);
int MyStrcmp (const char * First, const char *Second);
int MyStrncmp (const char *First, const char *Second, size_t max_n);
char *MyStrdup (const char *Orig);
size_t min (int a, int b);
void MyStrncpy (const char *Original, char *Parody, int n);
char *MyStrncat (char *destination, const char * to_paste, int n);
int MyAtoi (const char *Source);
char *MyFgets (char *input, int maxn, FILE *stream);

#endif // SOME_FUNCTION