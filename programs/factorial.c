#include <stdio.h>

/*
 * Recursive factorial function.
 * For n == 0 or 1, returns 1.
 * For n > 1, returns n * factorial(n - 1).
 */
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;               // Base case: 0! and 1! are 1
    }
    return n * factorial(n - 1); // Recursive case: n * (n-1)!
}

/*
 * Simple driver program:
 *  - chooses an input n
 *  - calls factorial(n)
 *  - prints the result
 *
 */
int main(void) {
    int n = 5;                              // Input value (can be changed)
    int result = factorial(n);              // Call recursive function

    printf("factorial(%d) = %d\n", n, result);

    return 0;
}
