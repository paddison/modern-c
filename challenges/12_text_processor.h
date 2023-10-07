#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H
#include <stdlib.h>
typedef struct blob blob;

// should text always contain /0 at the end?
struct blob {
    char* text;
    blob* prev;
    blob* next;
};

blob* split(size_t i, blob* split);
void join(blob* left, blob* right);
blob* lines(blob* start, char s[static 1]);
void print_lines(blob* start);

#endif
