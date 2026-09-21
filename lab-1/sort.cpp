#include <cstdio> // for printf and scanf
 

int main() 
{
    int numbers[10] = {9, 4, 5, 2, 3, 0, 8, 1, 7, 6};
    int val;

    printf("Here's the array that will be sorted:");

    // loop thru all values in numbers array
    for (int m = 0; m < 10; m++)
    {
        printf("%d ", numbers[m]);
    }

    printf("\nThese will now be sorted...");

    for (int i = 0; i < 10; i++) 
    {
        for (int j = i + 1; j < 10; j++) // had to change from j=1 to j=i+1 and j<i to j<10.
        {
            if (numbers[j] < numbers[i]) {
                val = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = val;
            }
        }
    }

    printf("\nHere's the sorted array:");

    for (int n = 0; n < 10; n++)
    {
        printf("%d ", numbers[n]); //added a space after %d so each number would seperate
    }
}


