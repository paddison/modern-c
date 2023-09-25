#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <stdio.h>
#include <string.h>


bool is_digit(char const c) {
    return c >= '0' && c <= '9';
}

int my_strtoi(char const str[static 1], const char** ptr) {
    *ptr = str;
    size_t n = 0;
    size_t const offset = 48;
    int sign_bit = 1;
    if (str[0] == '-') {
        sign_bit = -1;
        ++(*ptr);
    }

    for (;is_digit(**ptr); ++(*ptr)) {
        n = n * 10 + ((**ptr) - offset);
    }

    return n * sign_bit;
}

double my_strtod(char str[static 1], const char** ptr) {
    double sign_bit = str[0] == '-' ? -1 : 1;
    double decimal = 0.;
    double exponent = 0.;
    int decimal_width = 0;

    // determine the form of the string
    const char *dot = strchr(str, '.');
    const char *e = strchr(str, 'e');

    if (!e) e = strchr(str, 'E');

    // if the string starts with an e, don't parse it
    if (e == str) return 0.;

    // parse until '.'
    double num = abs(my_strtoi(str, ptr)); // always should be positive

    // if its just an integer
    if (!dot && !e) {
        return sign_bit * num;
    }

    // if the next symbol is '.', and the next character is a number
    if (*ptr == dot && is_digit(*((*ptr) + 1))) {

        decimal = my_strtoi(dot + 1, ptr); 
        // negative decimal place indicates invalid string
        // determine how many digits the decimal place has
        decimal_width = *ptr - dot - 1;
    }
    // if the next symbol is 'e' or 'E' and it either starts with a digit, +, or -
    if (*ptr == e && (
        is_digit(*((*ptr) + 1))) || 
        *((*ptr) + 1) == '-' || 
        *((*ptr) + 1) == '+') 
    {
        exponent = my_strtoi(e + 1, ptr);
    }
                        
    return (num + decimal * pow(10, -decimal_width)) * pow(10, exponent);
}                        

int main(int argc, char* argv[argc+1]) {
    if (argc > 1) {
        const char* ptr;
        for (size_t i = 1; i < argc; ++i) {
            printf("n: %f\n", my_strtod(argv[i], &ptr));
            printf("rest: %s", ptr);
        }
    }
}
                                
