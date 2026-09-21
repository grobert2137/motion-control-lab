#define _CRT_SECURE_NO_WARNINGS
#include <cstdio> 
#include <iostream>

// Function to sort array of int from smallest to largest
void sort(int* array, int length) {
    int val;
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < 10; j++) { 
            if (array[j] < array[i]) {
                val = array[i];
                array[i] = array[j];
                array[j] = val;
            }
        }
    }
}

// Function to print all values of array with known length
void print_array(int* array, int length) {
    for (int n = 0; n < length; n++)
    {
        printf("%d ", array[n]); //added a space after %d so each number would seperate
    }
}

int main()
{
    int numbers[10] = { 6, 4, 5, 2, 3, 0, 8, 1, 7, 9 };
    int length = sizeof(numbers) / sizeof(*numbers);

    // Original Array
    printf("Original array: ");
    print_array(numbers, length);
    
    // Sort array
    sort(numbers, length);

    // Print sorted array
    printf("\nHere's the sorted array:");
    print_array(numbers, length);
    printf("\n");
}




