#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define EPOSIX 5
#define ECLASS 6
#define ECLASSRANGE 7

static const size_t N_POSIX = 8;

static char* POSIX_CLASSES[N_POSIX][2] = {
    { "[:upper:]", "[A-Z]" },
    { "[:lower:]", "[a-z]" },
    { "[:alpha:]", "[A-Za-z]" },
    { "[:digit:]", "[0-9]" },
    { "[:xdigit:]", "[0-9A-Fa-f]" },
    { "[:alnum:]", "[A-Za-z0-9]" },
    { "[:blank:]", "[ \t]" },
    { "[:space:]", "[ \t\n\r\f\v]" },
};

typedef struct Match Match;

struct Match {
    size_t index;
    size_t len;
};

typedef enum RTag RTag;

enum RTag {
    ANY,
    CLASS,
};

typedef struct RegexNode RegexNode;

struct RegexNode {
    RTag tag;
    char* matches;
    size_t len;
    RegexNode* prev;
    RegexNode* next;
};

Match match(const char string[static 1], const char pat[static 1]);

int r_node_POSIX_to_char_class(RegexNode* node, 
                                const char pat[static 1], 
                                size_t cap);

void r_node_print(RegexNode* node) {
    for (;;) {
        switch (node->tag) {
            case ANY:
                printf("Any: *");
                break;
            case CLASS:
                printf("Class: ");
                for (size_t i = 0; i < node->len; ++i) {
                    putc(node->matches[i], stdout);
                }
                break;
            default:
                // TODO return error
                break;
        }
        if (!node->next) {
            break;
        } 
        printf("->");
        node = node->next;
    }
    printf("\n");
}

size_t r_node_match(RegexNode* node, const char string[static 1]) {
    // one node corresponds to one char
    size_t i = 0;
    for (; i < strlen(string); ++i) {
        if (!node) break;

        switch (node->tag) {
            case ANY:
                break;
            case CLASS: {
                bool matched = false;
                for (size_t j = 0; j < node->len; ++j) {
                    if (node->matches[j] == string[i]) {
                        matched = true;         
                        break;
                    }
                }
                if (!matched) return -1;
                break;
            }
        }

        node = node->next;
    }
    
    // if regex is not fully matched
    if (node) return -1;

    return i;
}

int r_node_parse_char_class_inner(RegexNode* node, 
                                   const char pat[static 1],
                                   size_t cap
                                   ) {
    // i points to the first character after '['
    assert(pat[0] == '[');
    size_t i = 1;
    
    while (pat[i] != ']') {
        if (pat[i] == '[') {
            int res = r_node_POSIX_to_char_class(node, pat + i, cap);
            if (res < 0) return res;
            i += res;
        } else if (pat[i + 1] == '-') {
            // check that range is valid
            if (pat[i + 2] == '-') return -ECLASSRANGE;
            if (pat[i] > pat[i + 2]) return -ECLASSRANGE;

            for (size_t c = pat[i]; c <= pat[i + 2]; ++c) {
                if (node->len == cap) {
                    cap <<= 1;
                    node->matches = realloc(node->matches, sizeof(char) * cap);
                }
                node->matches[node->len++] = c;
            }
            i += 3;
        } else {
            if (node->len == cap) {
                cap <<= 1;
                node->matches = realloc(node->matches, sizeof(char) * cap);
            }
            node->matches[node->len++] = pat[i];
            ++i;
        }

        if (!pat[i]) return -ECLASS;
    }
    return i;
}

int r_node_POSIX_to_char_class(RegexNode* node, 
                                const char pat[static 1], 
                                size_t cap) {
    // i points to '[' of posix class
    for (size_t j = 0; j < N_POSIX; ++j) {
        if (strspn(pat, POSIX_CLASSES[j][0]) == strlen(POSIX_CLASSES[j][0])) {
            int ret =  r_node_parse_char_class_inner(node, 
                    POSIX_CLASSES[j][1], cap);
            if (ret < 0) return ret;
            else return strlen(POSIX_CLASSES[j][0]);
        }
    }

    return -EPOSIX;
}

size_t r_node_parse_char_class(RegexNode* node, const char pat[static 1]) {
    node->tag = CLASS;
    node->len = 0;
    size_t cap = 8;
    node->matches = malloc(sizeof(char) * cap); 
    // i points to '['
    assert(pat[0] == '[');

    return r_node_parse_char_class_inner(node, pat, cap);

}

RegexNode* r_node_parse(const char pat[static 1]) {
    RegexNode* cur = 0;
    RegexNode* prev = 0;

    size_t len = strlen(pat);

    for (size_t i = 0; i < len; ++i) {
        cur = malloc(sizeof(RegexNode));

        switch (pat[i]) {
            case '*':
                cur->tag = ANY;
                cur->matches = 0;
                cur->len = 0;
                break;
            case '[': {
                int result = r_node_parse_char_class(cur, pat + i);
                if (result < 0) {
                    // do something with error 
                }
                i += result; 
                break;
                      }
            case '?':
                break;
            default: 
                cur->tag = CLASS;
                cur->matches = malloc(sizeof(char));
                *cur->matches = pat[i];
                cur->len = 1;
                break;
        }

        cur->prev = prev;
        if (prev) prev->next = cur;
        prev = cur;
    }

    cur->next = 0;

    while(cur->prev) {
        cur = cur->prev;
    }

    return cur;
}

size_t find_word(const char string[static 1], 
                 const char word [static 1]) {
    size_t string_len = strlen(string);
    size_t word_len = strlen(word);

    for (size_t i = 0; i <= string_len - word_len; ++i) {
        if (string[i] == word[0] && strspn(string + i, word) == word_len) {
            return i; 
        }
    }

    return -1;
}

char* find_and_replace(const char string[static 1], 
                       const char pat[static 1],
                       const char replace[static 1]) {
    Match m = match(string, pat);

    if (m.len == 0) return 0;

    size_t start = m.index;

    /* allocate space for the new string */
    size_t string_len = strlen(string);
    size_t find_len = m.len;
    size_t replace_len = strlen(replace);
    size_t new_len = string_len + replace_len - find_len;
    char* new = malloc((new_len + 1) * sizeof(char));

    /* copy the beginning of the string */
    memcpy(new, string, start);

    /* copy the replacing word */
    memcpy(new + start, replace, replace_len);

    /* copy the rest of the string */
    memcpy(new + start + replace_len, 
           string + start + find_len, 
           string_len - start - find_len);

    new[new_len] = 0;

    return new;
}

Match match(const char string[static 1], const char pat[static 1]) {
    RegexNode* node = r_node_parse(pat);
    for (size_t i = 0; i < strlen(string); ++i) {
        size_t res = r_node_match(node, string + i); 
        if (res != -1) {
            return (Match) { .index = i, .len = res };
        }
    }
    return (Match) { .index = -1, .len = 0 };
}

int main(void) {
    char* string = "hello, my name is pat";
    char* word_contained = "name";
    char* word_not_contained = "namee";
    char* word_contained_last = "pat";

    assert(find_word(string, word_contained) == 10);
    assert(find_word(string, word_not_contained) == -1);
    assert(find_word(string, word_contained_last) == 18);

    char* replace = "mein";
    char* find = "my";
    char* new = find_and_replace(string, find, replace);
    printf("%s\n", new);

    char* replace_end = "paddison";
    char* find_end = "pat";
    char* new_end = find_and_replace(string, find_end, replace_end);
    printf("%s\n", new_end);

    char* pat = "[[:digit:]h-m]*o";
    printf("%zu", match("hello,", pat).index);
    return EXIT_SUCCESS;
}
