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

size_t Find(size_t vertex, size_t N, segment regions[N]) {
    while (regions[vertex].parent != vertex) {
        vertex = regions[vertex].parent;
    }
    return vertex;
}

void Union(size_t root, size_t vertex, size_t N, segment regions[N]) {
    if (regions[root].parent != root || regions[vertex].parent != vertex) {
        return;
    }
    regions[vertex].parent = root; 
    regions[root].n_pixels += regions[vertex].n_pixels;
    regions[root].sum += regions[vertex].sum;
}

bool can_merge(segment l, segment r, double const delta) {
    double l_avg = (double) l.sum / (double) l.n_pixels;
    double r_avg = (double) r.sum / (double) r.n_pixels;
    return fabs(l_avg - r_avg) <= delta;
}

bool merge(size_t vertex, 
                  size_t adjacent, 
                  double delta, 
                  size_t size, 
                  segment regions[size]) {
    // get the roots
    size_t root_vertex = Find(vertex, size, regions);
    size_t root_adjacent = Find(adjacent, size, regions);
    if (root_vertex == root_adjacent || !can_merge(regions[root_vertex], regions[root_adjacent], delta)) 
        return false;

    size_t adjacent_root = Find(adjacent, size, regions);
    assert(adjacent_root != vertex);

    Union(root_adjacent, root_vertex, size, regions);
    return true;
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
    segment regions[width * height];

    printf("width: %d, height: %d\n", 
        width, 
        height);

    // parse image, since we want greyscale, average rgb values
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


    // merge the regions
    size_t merged_count;

    do {
        merged_count = 0;

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                size_t vertex = x + y * width;
                // try to merge to the left
                if (x > 0 &&  merge(vertex, vertex - 1, delta, width * height, regions))
                    ++merged_count;
                // try to merge to the top
                if (y > 0 && merge(vertex, vertex - width, delta, width * height, regions))
                    ++merged_count;
            }
        }
        printf("Merged %zu regions\n", merged_count);
    } while (merged_count > 0);

    printf("Done merging.\n");

    // write segments to file
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            segment root = regions[Find(x + y * width, width * height, regions)];
            unsigned char px = root.sum / root.n_pixels;
            set_pixel_rgb(bmp, x, y, px, px, px);
        }
    }

    bwrite(bmp, "out.bmp");
    bclose(bmp);

    printf("Wrote output to out.bmp\n");
    return EXIT_SUCCESS;
}
