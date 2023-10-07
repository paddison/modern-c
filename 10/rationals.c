#include "rationals.h"
#include "tgmath.h"
rat rat_get(long long num, unsigned long long denom) {
    rat ret = {
        .sign = (num < 0),
        .num = (num < 0) ? -num : num,
        .denom = denom,
    };
    return ret;
}

rat rat_get_normal(rat x) {
    size_t c = gcd(x.num, x.denom);
    x.num /= c;
    x.denom /= c;
    return x;
}

rat rat_get_extended(rat x, size_t f) {
    x.num *= f;
    x.denom *= f;
    return x;
}

rat rat_get_prod(rat x, rat y) {
    // normalize first, since denominator might overflow
    x = rat_get_normal(x);
    y = rat_get_normal(y);
    rat ret = {
        .sign = (x.sign != y.sign),
        .num = x.num * y.num,
        .denom = x.denom * y.denom,
    };
    return rat_get_normal(ret);
}
                
rat rat_get_sum(rat x, rat y) {
    size_t c = gcd(x.denom, y.denom);
    size_t ax = y.denom/c;
    size_t bx = x.denom/c;
    x = rat_get_extended(x, ax);
    y = rat_get_extended(y, bx);
    assert(x.denom == y.denom);
    if (x.sign == y.sign) {
        x.num += y.num;
    } else if (x.num > y.num) {
        x.num -= y.num;
    } else {
        x.num = y.num - x.num;
        x.sign = !x.sign;
    }
    return rat_get_normal(x);
}

void rat_destroy(rat* rp) {
    if (rp) *rp = (rat){ 0 }; // set all struct fields to 0
}

rat* rat_init(rat* rp,
              long long num,
              unsigned long long denom) {
    if (rp) *rp = rat_get(num, denom);
    return rp;
}

rat* rat_normalize(rat* rp) {
    if (rp) *rp = rat_get_normal(*rp);
    return rp;
}

rat* rat_extend(rat* rp, size_t f) {
    if (rp) *rp = rat_get_extended(*rp, f);
    return rp;
}

rat* rat_rma(rat* rp, rat x, rat y) {
    return rat_sumup(rp, rat_get_prod(x, y));
}

rat* rat_sumup(rat* rp, rat y) {
    size_t c = gcd(rp->denom, y.denom);
    size_t ax = y.denom / c;
    size_t bx = rp->denom / c;
    rat_extend(rp, ax);
    y = rat_get_extended(y, bx);
    assert(rp->denom == y.denom);

    if (rp->sign == y.sign) {
        rp->num += y.num;
    } else if (rp->num > y.num) {
        rp->num -= y.num;
    } else {
        rp->num = y.num - rp->num;
        rp->sign = !rp->sign;
    }
    return rat_normalize(rp);
}

char const* rat_print(size_t len, char tmp[len], rat const* x) {
    char sign = x->sign ? '+' : '-'; 
    // calculate the length needed to store the string
    double num_width = (unsigned) log10(x->num);
    double denom_width = (unsigned) log10(x->denom); 

    if (1 + num_width + 1 + denom_width >= len) {
        fprintf(stderr, "capacity not enough to store number");
    }

    sprintf(tmp, "%c, %zu, %zu", sign, x->num, x->denom);
    return tmp;
}


char const* rat_normalize_print(size_t len, char tmp[len],
                                rat const* x) {
    rat n_rat = rat_get_normal(*x);
    return rat_print(len, tmp, &n_rat);
}

rat* rat_dotproduct(rat rp[static 1], size_t n,
                    rat const A[n], rat const B[n]) {
    rat_destroy(rp);
    for (size_t i = 0; i < n; ++i) {
        rat_rma(rp, A[i], B[i]);
    }
    return rp;
}
