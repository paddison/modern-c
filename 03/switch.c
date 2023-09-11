#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char arg = 'm';

    // switch __jumps__ to a specified case
    switch(arg) {
        case 'm': puts("This is a magpie.");
                  break;          
        int j = 5;
        puts("no case");
        case 'r': puts("This is a raven.");
                  //break;
        // case 'm': puts("another magpie."); a case has to be unique!
        case 'j': puts("This is a jay.");
                  break;
        case 'c': puts("This is a chough.");
                  int i = 1;
                  printf("%i\n", i);
        default:  puts("This is an unknown corvid.");
                  printf("i:%i, j:%i\n", i, j);
    }

    return EXIT_SUCCESS;
}