#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

enum { buf_max = 32, };

void print_file(char buffer[buf_max], char file_name[static 1], int* ret, bool line_numbers) {
    FILE* instream = fopen(file_name, "r");
    bool is_full = false;

    if (instream) {
        size_t line = 0;
        while (fgets(buffer, buf_max, instream)) {
            if (line_numbers && !is_full) {
                printf("%zu: ", line);
                ++line;
            } 
            fputs(buffer, stdout);
            is_full = strlen(buffer) == 31; // is there no better way to do this?
        }
        fclose(instream);
        ret = EXIT_SUCCESS;
    } else {
        /* Provide some error diagnostic. */
        fprintf(stderr, "Could not open %s: ", file_name);
        perror(0);
        errno = 0;
    }
}

int main(int argc, char* argv[argc+1]) {
    int ret = EXIT_FAILURE;
    char buffer[buf_max] = { 0 };
    bool line_numbers = false;

    if (argc > 1 && !strcmp(argv[1], "-n")) {
        line_numbers = true;
    }

    if (argc == 1 || line_numbers && argc == 2) {
        char file_name[128];
        printf("Enter file name: ");
        fgets(file_name, 128, stdin);
        file_name[strcspn(file_name, "\n")] = 0;
        print_file(buffer, file_name, &ret, line_numbers);        
    } else {
        for (int i = 1; i < argc; ++i) {
            print_file(buffer, argv[i], &ret, line_numbers);
        }
        
    }
    return ret;
}

// Exs 4: This program will return SUCCESS if it at least successfully 
//     reads in one file.
// Exs 5: fgets seems to read until the buffer is full, or until it encounters a new line
//     so if a line is longer than 31 characters, it just puts it out, and then continues
//     reading the line until it eventually reads the newline character.
