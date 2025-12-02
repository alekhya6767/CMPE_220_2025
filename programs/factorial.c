// ============================================================
// Simple C program that uses recursion to compute factorial.
// This is the *conceptual* reference for our software CPU
// factorial.asm program (they implement the same logic).
// ============================================================

#include <stdio.h>

// ------------------------------------------------------------
// Recursive factorial function
//
// Mathematically:
//   fact(0) = 1
//   fact(n) = n * fact(n - 1)   for n > 0
//
// This matches what we implemented in factorial.asm with
// CALL / RET and a stack frame that saves 'n'.
// ------------------------------------------------------------
int factorial(int n) {
    // Base case: by definition, 0! = 1
    if (n == 0) {
        return 1;
    }

    // Recursive case:
    //   factorial(n) = n * factorial(n - 1)
    //
    // Each call:
    //   - pushes the return address on the call stack
    //   - stores 'n' in the current frame
    //   - calls factorial(n - 1)
    //   - multiplies the returned value by n
    //
    // In our software CPU, the same thing happens using:
    //   PUSH (to save n)
    //   CALL fact
    //   POP  (to restore n)
    //   then a multiplication loop.
    return n * factorial(n - 1);
}

// ------------------------------------------------------------
// Driver program (main)
//
// This plays the role of "caller" in C, just like:
//
//   MOVI R0, 5
//   CALL fact
//   STORE R0, 0xFF00
//
// in factorial.asm. Here we just print the result using printf.
// ------------------------------------------------------------
int main(void) {
    int n = 5;                  // choose the input n
    int result = factorial(n);  // call the recursive function

    // Print result to the console.
    // Equivalent to writing the result to memory-mapped I/O
    // at address 0xFF00 in our software CPU.
    printf("factorial(%d) = %d\n", n, result);

    return 0;   // indicate successful program termination
}
