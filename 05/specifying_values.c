#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // exs 1:
    // printf("-9223372036854775808 %lld\n", -9223372036854775808LL); // doesn't even compile
    printf("-9223372036854775808: %lld\n", -9223372036854775807LL - 1LL); // this way it is possible
    printf("-2147483648: %ld\n", -2147483648L); // since 2147483648 can not be represented by long, it will be promoted to long long and thus be of type long long
    // exs 2:

    // exs 3:
    printf("-1:, %u\n", -1U);
    printf("-1:, %lu\n", -1UL);
    printf("-1:, %llu\n", -1ULL);

    return EXIT_SUCCESS;
}