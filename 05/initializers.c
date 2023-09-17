int main(void) {
    // All variables should be initialized.
    // The only exception are variable length arrays (VLA)

    // scalar types can be initialized with just expressions:
    // they can be optionaly wrapped in `{}`
    double a = 7.8;
    double b = 2 * a;
    double c = { 7.8 };
    double d = { 0 };

    // For other types, initializers need to have `{}`:
    double A[] = { 7.8 }; // if the length is omitted, it is deducted from the initializer expression
    double B[3] = { 2* A[0], 7, 33 };
    double C[] = { [0] = 6, [3] = 1 };

    // tere is a `default initializer` which is just { 0 }
    // it is valid for ALL object types that are not VLA.
    // they are usefull if we don't know how to initialize a variable
}