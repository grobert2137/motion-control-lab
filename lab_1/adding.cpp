//#define _CRT_SECURE_NO_WARNINGS // supress MSVC warnings
#include <stdio.h> // need stdio since no cstdio on Clang(Mac)

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