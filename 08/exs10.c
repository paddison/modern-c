#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Supposes that lowercase characters are contiguous. */
static_assert('z' - 'a' == 25, 
              "alphabetic characters not contiguous");

/* Converts an alphanumeric digit to an unsigne */
/* '0' ... '9'  =>  0 ..  9u */
/* 'A' ... 'Z'  => 10 .. 35u */
/* 'a' ... 'z'  => 10 .. 35u */
/* Other values =>   Greater */
unsigned hexatridecimal (int a) {
    if (isdigit(a)) {
        /* This is guaranteed to work: decimal digits 
           are consecutive, and isdigit is not
           locale dependent. */
        return a - '0';
    } else {
        /* Leaves a unchanged if it is not lowercase */
        a = toupper(a);
        /* Returns value >= 36 if not Latin uppercase */
        return (isupper(a)) ? (10 + (a >= 'A' ? a - 'A' : 'A' - a)) : -1;
    }
}

size_t find_prefix(char const s[static 1], 
                   size_t i, 
                   char const prefix[static 1]) {
    size_t p = 0;
    while(s[i] == prefix[p]) {
        ++p;
        ++i;
    }
    return p;
}

unsigned long Strtoul_inner(char const s[static 1],
                            size_t i,
                            unsigned base) {
    unsigned long ret = 0;
    while (s[i]) {
        unsigned c = hexatridecimal(s[i]);
        if (c >= base) break;
        /* Maximal representable value for 64 bit is
           3w5e11264sgsf in base 36 */
        if (ULONG_MAX / base < ret) {
            ret = ULONG_MAX;
            errno = ERANGE;
            break;
        }
        ret *= base;
        ret += c;
        ++i;
    }
    return ret;
}

unsigned long Strtoul(char const s[static 1], unsigned base) {
    if (base > 36u) {
        errno = EINVAL;
        return ULONG_MAX;
    }
    size_t i = strspn(s, " \f\n\r\t\v"); // Skips spaces
    bool switch_sign = false;            // Looks for a sign
    switch(s[i]) {
        case '-': switch_sign = true;
        case '+': ++i;      
    }
    if (!base || base == 16) {           // Adjusts the base
        size_t adj = find_prefix(s, i, "0x");
        if (!base) base = (unsigned[]){ 10, 8, 16,}[adj];        
        i += adj;
    }
    // Now, starts the real conversion 
    unsigned long ret = Strtoul_inner(s, i, base);
    return (switch_sign) ? -ret : ret;
}

int main(int argc, char* argv[argc+1]) {
    for (size_t i = 1; i < argc; ++i) {
        printf("%lu\n", Strtoul(argv[i], 0));
    }
    return EXIT_SUCCESS;
}
