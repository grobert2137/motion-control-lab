// Program to check addition of hexadecimal numbers
#include <stdio.h> // need stdio
//#include <cstdio> this should work on windows I think

int main() {

    //declares 
    int augend;
    int addend;
    int sum;
    printf("\nInput the augend in hex: ");
    scanf("%x", &augend); // use %x for hex, not %d

    printf("\nInput the addend in hex: ");
    scanf("%x", &addend);

    sum = augend + addend;
    printf("ANS: \n");
    printf("Sum (hex): %x\n", sum);
    printf("Sum (dec): %d\n", sum);
}