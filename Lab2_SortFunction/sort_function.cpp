#include <cstdio> // for printf and scanf
#include <iostream>

void sort(int* array, int length) { // sort function for 10 value array
    int val;

    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < 10; j++) { // had to change from j=1 to j=i+1 and j<i to j<10.
            if (array[j] < array[i]) {
                val = array[i];
                array[i] = array[j];
                array[j] = val;
            }
        }
    }
}

void print_array(int* array, int length) {
    for (int n = 0; n < length; n++)
    {
        printf("%d ", array[n]); //added a space after %d so each number would seperate
    }
}

int main()
{
    int numbers[10] = { 9, 4, 5, 2, 3, 0, 8, 1, 7, 6 };

    int length = sizeof(numbers) / sizeof(*numbers);

    sort(numbers, length);

    printf("\nHere's the sorted array:");
    print_array(numbers, length);
}



