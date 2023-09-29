#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include "cbmp.h"

typedef struct segment segment;
struct segment {
    size_t parent;
    unsigned char value;
    size_t n_pixels;
    size_t sum;
};

bool can_merge(segment l, segment r, double const delta) {
    double l_avg = (double) l.sum / (double) l.n_pixels;
    double r_avg = (double) r.sum / (double) r.n_pixels;

    if (l_avg > r_avg) {
        return l_avg - r_avg < delta;
    } else {
        return r_avg - l_avg < delta;
    }
}

size_t Find(size_t vertex, size_t N, segment regions[N]) {
    while (regions[vertex].parent != vertex) {
        vertex = regions[vertex].parent;
    }
    return vertex;
}

bool Union(size_t root, size_t vertex, double delta, size_t N, segment regions[N]) {
    size_t parent = Find(vertex, N, regions);
    if (parent == root) {
        return false;
    }
    if (!can_merge(regions[root], regions[parent], delta)) {
        return false;
    }
    regions[parent].parent = root; 
    regions[root].n_pixels += regions[parent].n_pixels;
    regions[root].sum += regions[parent].sum;
    return true;
}

void print(size_t width, size_t height, unsigned char M[height * width]) {
    for (size_t r = 0; r < height; ++r) {
        printf("[");
        for (size_t c = 0; c < width; ++c) {
            printf("%u ", M[r * width + c] / 10);
        }
        printf("]\n");
    }
}

/* 
*  When compiling pass -Wl,-stack_size -Wl,0x20000000
*  to increase the stack size to 32mb 
*/
int main(int argc, char* argv[static 1]) {
    if (argc != 3) {
        printf("Usage: cbmp file delta\n");
        return EXIT_FAILURE;
    }

    printf("Opening file: %s\n", argv[1]);

    BMP* bmp = bopen(argv[1]);
    double delta = strtof(argv[2], 0);
    if (delta > 255. || delta < 0.) {
        fprintf(stderr, "delta has to be between 0 and 255");
    }
    if (!bmp) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror(0);
        errno = 0;
    }

    // get picture dimension
    unsigned int width = get_width(bmp);
    unsigned int height = get_height(bmp);
    unsigned long size = height * width;
    segment regions[size];

    printf("width: %d, height: %d\n", 
        width, 
        height);

    // read in data
    // since we want greyscale, average rgb values
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            unsigned char r, g, b;
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
            unsigned char greyscale = (r + g + b) / 3;
            size_t index = x + width * y;
            regions[index] = (segment) {
                .parent = index,
                .value = greyscale,
                .n_pixels = 1,
                .sum = greyscale
            };
        }
    }
    printf("Read in file data.\n");

    size_t merged_count = 1;
    while (merged_count > 0) {
        merged_count = 0;
        for (size_t i = 0; i < size; ++i) {
            // only consider roots
            if (regions[i].parent != i) continue;

            // always merge up and left. this way, a root will always have a 
            // non root as adjacent region
            if (i % width > 0) {
                // get region to the left
                size_t left = Find(i - 1, size, regions);
                assert(left != i);
                if (Union(left, i, delta, size, regions)) {
                    ++merged_count;
                }
            }
        }    
        for (size_t i = 0; i < size; ++i) {
            // only consider roots
            if (regions[i].parent != i) continue;

            // always merge up and left. this way, a root will always have a 
            // non root as adjacent region
            if (i / width > 0) {
                // get region to the top
                size_t left = Find(i - width, size, regions);
                assert(left != i);
                if (Union(left, i, delta, size, regions)) {
                    ++merged_count;
                }
            }
        }    
        printf("Merged %zu regions\n", merged_count);
    }

    printf("Done merging.\n");

    // write segments to file
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            segment root = regions[Find(x + y * width, size, regions)];
            unsigned char px = root.sum / root.n_pixels;
            set_pixel_rgb(bmp, x, y, px, px, px);
        }
    }
    bwrite(bmp, "out.bmp");
    printf("wrote output to out.bmp\n");
    bclose(bmp);
    return EXIT_SUCCESS;
}
