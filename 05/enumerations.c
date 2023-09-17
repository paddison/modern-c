#include <stdio.h>

// declaring an enum
// they are ordered and of type signed int (starting from 0)
// if we'd decide to add another bird later, we could simply add another variant
enum corvid { magpie, raven, jay, corvid_num };

// Explicitly specify their value:
enum { p0 = 1, p1 = 2*p0, p2 = 2*p1, p3 = 2*p2 };

// when defining these values, we can use integer constant expressions (ICE)
// they are a compile-time integer value
// spcifically, the value must be known at compile time and NO object evaluation 
// can happen during that process:
signed const o42 = 42;
enum  {
    b42 = 42,
    c52 = o42 + 10, // this shouldn't work, but for some reason, clang can still evaluate it
    b52 = b42 + 10,
};

// In C, the only value a compile time constant can have is signed int.

// To work around this limitation, we can employ macros:
// macros are basically textual replacement defined by preprocessor statements:
// By convention, macro names are always in CAPS
# define M_PI 3.14159265358979323846

// things like SIZE_MAX are defined with macros.    

// if a type doesn't simply have a literal which describes its constant,
// we have to use `compound literals` in the form of (T){ INIT }:
# define CORVID_NAME /**/           \
(char const*const[corvid_num]) {    \
    [raven] = "raven",              \
    [magpie] = "magpie",            \
    [jay] = "jay",                  \
}

// Note that compound literals define objects though
// There must also be a space between the macro name and the type definition,
// otherwise it would be interpreted as a functin-like_macro
// the `\` allows the macro definition to span several lines
// there can be no `;` at the end (remember, it is just text replacement)

// it is also considered good practice to right indent the `\` to improve readability
int main(void) {

    // since array initializers don't need to be ordered, we could add another enum variant
    // simply add the top.
    char const*const bird[corvid_num] = {
        [raven] = "raven",
        [magpie] = "magpie",
        [jay] = "jay",
    };

    for (unsigned i = 0; i < corvid_num; ++i) {
        printf("Corvid %u is the %s\n", i, bird[i]);
    }
}