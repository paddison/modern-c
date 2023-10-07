#include "12_text_processor.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


blob* init_blob() {
    return malloc(sizeof(blob));
}

void delete_blob(blob* b) {
    free(b->text);
    free(b);
}

char* new_string(size_t length, char* source) {
    char* new_string = malloc(sizeof(char) * length + 1); 
    memcpy(new_string, source, length);
    new_string[length] = 0;

    return new_string;
}
// i points to the character AFTER the new line
// splits "split" at i, appending a new blob to the right
// the original blob is preserved
blob* split(size_t i, blob *split) {
    size_t text_len = strlen(split->text);
    assert(text_len > i);

    blob* right = init_blob();

    // copy strings and set pointers
    right->text = new_string(text_len - i, split->text + i);
    right->prev = split;
    right->next = split->next;

    char* split_text = new_string(i, split->text);

    free(split->text);
    split->text = split_text;
    split->next = right;

    return right;
}

// join right into left
void join(blob *left, blob *right) {
    assert(left->next == right);

    size_t l_len = strlen(left->text); 
    size_t r_len = strlen(right->text);

    char* new_text = malloc(sizeof(char) * l_len + r_len + 1);
    memcpy(new_text, left->text, l_len);
    memcpy(new_text + l_len, right->text, r_len);
    new_text[l_len + r_len] = 0;

    left->text = new_text;
    left->next = right->next;    

    free(left->text);
    delete_blob(right);
}



blob* lines(blob* start, char text[static 1]) {
    size_t text_len = strlen(text);
    start->text = malloc(sizeof(char) * text_len + 1);

    strcpy(start->text, text);
    // determine position of '\n' 
    char* i;
    while ((i = strchr(start->text, '\n'))) {
        start = split(i + 1 - start->text, start);
    }

    return start;
}

void print_lines(blob *start) {
    assert(!start->prev);
    blob* current = start;

    while(current) {
        printf("%s", current->text);
        current = current->next;
    }
}

int main(void) {
    // store blobs in array since we cannot do heap allocation yet
    char* test = "this is a text\nit has four lines\nthis is 3\nthis is 4";

    blob* start = init_blob();
    lines(start, test);

    print_lines(start);
}
