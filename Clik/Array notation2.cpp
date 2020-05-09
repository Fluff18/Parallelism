/*
 * check-for-5.cpp
 *
 * Demonstrate using an if statement with elements of an array using
 * both a for loop and Array Notation
 */

#include 
#include 

const int array_size=10;

int main(int argc, char **argv)
{
    int a[array_size];
    const char *loop_results[array_size];
    const char *an_results[array_size];

    // Initialize array using for loop
    for (int i = 0; i < array_size; i++)
        a[i] = i;

    // Check for 5 using a for loop
    printf ("Check for array element containing 5 using for loop:\n");
    for (int i = 0; i < array_size; i++)
    {
        if (5 == a[i])
            loop_results[i] = "Matched";
        else
            loop_results[i] = "Not Matched";
    }

    // Check for 5 using Array Notation
    printf ("\nCheck for array element containing 5 using Array Notation:\n");
    if (5 == a[:])
        an_results[:] = "Matched";
    else
        an_results[:] = "Not Matched";

    // Verify the results
    bool success = true;
    for (int i = 0; i < array_size; i++)
    {
        if (strcmp(loop_results[i], an_results[i]))
            success = false;
    }

    if (success)
        printf("Results match\n");
    else
        printf("Results differ!\n");

    return 0;
}