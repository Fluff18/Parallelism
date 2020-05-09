/*
 * set-2d-stride.cpp
 *
 * Demonstrate setting all elements of a 2D array to a value using both for loop
 * and Array Notation
 */

#include <stdio.h>
#include <string.h>

const int array_size=10;

int main(int argc, char **argv)
{
    int i, j;
    int a[array_size][array_size];
    int b[array_size][array_size];

    // Initialize array using for loops
    for (i = 0; i < array_size; i++)
        for (j = 0; j < array_size; j++)
            a[i][j] = 5;

    // Set even rows using for loop
    for (i = 0; i < array_size; i += 2)
    {
        for (j = 0; j < array_size; j++)
            a[i][j] = 12;
    }

    // Initialize the array using Array Notation.  Since the array is
    // statically allocated, we can use default values for the start index (0)
    // and number of elements (all of them).
    b[:][:] = 5;

    // Set elements of even rows to 12
    b[0:5:2][:] = 12;

    // Verify the results - The arrays should be identical
    if (0 == memcmp(a, b, sizeof(a)))
        printf("Success\n");
    else
        printf("Failed\n");

    return 0;
}