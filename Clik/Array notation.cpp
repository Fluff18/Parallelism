/*
 * letter-type-if.cpp
 *
 * Demonstrate testing each element of an array using  both for loop and Array
 * Notation
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char a[] = "Hello world";

    printf ("Check letter types using for loop:\n");
    for (int i = 0; i < sizeof(a); i++)
    {
        if (('a' == a[i]) ||
            ('e' == a[i]) ||
            ('i' == a[i]) ||
            ('o' == a[i]) ||
            ('u' == a[i]))
            printf("%c: Vowel %d\n", a[i], a[i]);
        else
        {
            if ('\0' == a[i])
                printf("   NUL\n");
            else
            {
                if (' ' == a[i])
                    printf("   Space\n");
                else
                    printf("%c: Consonant: %d\n", a[i], a[i]);
            }
        }
    }

    printf ("\nCheck letter types using Array Notation:\n");
    if (('a' == a[:]) ||
        ('e' == a[:]) ||
        ('i' == a[:]) ||
        ('o' == a[:]) ||
        ('u' == a[:]))
        printf("%c: Vowel: %d\n", a[:], a[:]);
    else
    {
        if ('\0' == a[:])
            printf("   NUL\n");
        {
            if (' ' == a[:])
                printf("   Space\n");
            else
                printf("%c: Consonant: %d\n", a[:], a[:]);
        }
    }

    return 0;
}