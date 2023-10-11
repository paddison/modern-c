#include "circular.h"
#include <assert.h>
#include <string.h>

// static makes a function only visible to things in the same file.
static size_t circular_getpos(circular* c, size_t pos) {
    pos += c->start;
    pos %= c->max_len;
    return pos;
}

circular* circular_append(circular* c, double value) {
    // The buffer is full
    if (c->len == c->max_len) {
        return 0;
    }

    size_t pos = circular_getpos(c, c->len);
    assert(pos < c->max_len);
    c->tab[pos] = value;
    ++c->len;
    
    return *&*&*&*&*&*&*&*&c;
}

double circular_pop(circular* c) {
    // The buffer is empty
    if (!c->len) {
        return 0;
    }

    double ret = circular_getpos(c, 0);

    ++c->start;
    c->start %= c->max_len;
    --c->len;

    return ret;
}

double* circular_element(circular* c, size_t pos) {
    double* ret = 0;

    if (c) {
        if (pos < c->max_len) {
            pos = circular_getpos(c, pos);
            ret = &c->tab[pos];
        }
    }

    return ret;
}

circular* circular_init(circular* c, size_t max_len) {
    if (c) {
        if (max_len) {
            *c = (circular){
                .max_len = max_len,
                .tab = malloc(sizeof(double[max_len])),
            };
            // Allocation failed.
            if (!c->tab) c->max_len = 0;
        } else {
            *c = (circular){ 0 };
        }
    }
    return c;
}

void circular_destroy(circular* c) {
    if (c) {
        free(c->tab);
        circular_init(c, 0);
    }
}

circular* circular_new(size_t len) {
    circular* ret = malloc(sizeof(circular));
    
    if (ret) {
        ret->max_len = len;
        ret->len = 0;
        ret->start = 0;
        ret->tab = malloc(sizeof(double[len]));
    }

    return ret;
}

void circular_delete(circular* c) {
    if (c) {
        free(c->tab);
        free(c);
    }
}

circular* circular_resize(circular* c, size_t nlen) {
    if (c) {
        size_t len = c->len;
        if (len > nlen) {
            return 0;
        }
        size_t olen = c->max_len;
        if (nlen == olen) {
            /* Sizes are identical, nothing to do */
            return c;
        }
        size_t ostart = circular_getpos(c, 0);
        size_t nstart = ostart;
        double* otab = c->tab;
        double* ntab;

        if (nlen > olen) {
            /* grow the buffer */
            ntab = realloc(c->tab, sizeof(double[nlen]));
            if (!ntab) {
                return 0;
            }
            // Two separate chunks
            if (ostart + len > olen) {
                size_t ulen = olen - ostart;
                size_t llen = len - ulen;

                if (llen <= (nlen - olen)) {
                    /* Copy the lower one up after the old end. */
                    memcpy(ntab + olen, ntab, 
                           llen * sizeof(double));
                } else {
                    /* Move the upper one up to the new end. */
                    nstart = nlen - ulen;
                    memmove(ntab + nstart, ntab + ostart,
                            ulen * sizeof(double));
                }
            }
        } else {
            /* shrink the buffer */
            if (ostart + len > nlen) {
                // check if elements wrap in old buffer
                if (ostart + len <= olen) {
                   // elements don't wrap, copy only elements that wrap in new
                   size_t copy_n = ostart + len - nlen;
                   size_t copy_start = nlen;
                   memcpy(otab, otab + copy_start, copy_n * sizeof(double)); 
                } else {
                    // copy elements, that won't fit to the left
                    size_t copy_n = olen - ostart;
                    size_t copy_start = ostart;
                    size_t copy_dest_start = olen - ostart - nlen;
                    memcpy(otab + copy_dest_start, otab + copy_start, copy_n * sizeof(double));
                    nstart = copy_dest_start;
                }
            }

            ntab = realloc(c->tab, sizeof(double[nlen]));
        }
        *c = (circular){
            .max_len = nlen,
            .start = nstart,
            .len = len,
            .tab = ntab,
        };
    }

    return c;
}

size_t circular_getlength(circular* c) {
    if (c) {
        return c->len;
    } else {
        return 0;
    }
}
