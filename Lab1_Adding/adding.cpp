// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio> // need stdio since no cstdio on Clang(Mac)

int main() {

    //declares 
    int augend;
    int addend;
    int sum;
    printf_s("\nInput the augend in hex: ");
    scanf_s("%x", &augend); // use %x for hex, not %d

    printf_s("\nInput the addend in hex: ");
    scanf_s("%x", &addend);

    sum = augend + addend;
    printf_s("ANS: \n");
    printf_s("Sum (hex): %x\n", sum);
    printf_s("Sum (dec): %d\n", sum);
}
