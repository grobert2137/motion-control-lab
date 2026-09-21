#include <stdio.h>
#include <math.h> // for pow()

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

double getSinApprox(double x, double terms) {
    double result = 0.0;
    for (int j = 0; j < terms; j++) {
        double f = fact(2*j+1);
        result += ((pow(-1.0, j)*pow(x, (2*j+1)))/f);
        printf("\n%lf", result);
    }
    return result;
}

int main() { // take number input and get power series
    double x_input;
    double n_input;

    printf("\nGive me X in degrees: ");
    scanf("%lf", &x_input);
    printf("\nGive me N: ");
    scanf("%lf", &n_input);

    double ans = getSinApprox(((x_input/180.0)*3.14), n_input);
    printf("\nAnswer is: %lf \n", ans);

    return 0;
}