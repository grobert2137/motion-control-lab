#include "fact.h"

double fact(int n) { 
    
    // Negative input condition
    if (n < 0) { // factorial is undefined
        return -1.0;
    }
    
    // zero input condition
    else if (n == 0) {
    
    // factorial is 1
        return 1;
    }
    // standard factorial computation
    else {
        double factorial = 1.0;
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
    return factorial;
    }
}