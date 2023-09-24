#include <stdio.h>
#include <stdlib.h>

int main(void) {
    if (!freopen("mylog.txt", "a", stdout)) {
        perror("freopen failed");
        return EXIT_FAILURE;
    }
    puts("feeling fine today");
    return EXIT_SUCCESS;
}
