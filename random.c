#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    char *str;
    int Arrival;
    int Burst_TIme;
    scanf("%s", str);
    for (int j = 0; j < 3; j++)
    {
        printf("Test case %d:\n", j + 1);
        printf("5\n");
        if (strcmp(str, "RR") == 0)
        {
            printf("10\n");
        }
        for (int i = 0; i < 5; i++)
        {
            Arrival = rand() % 20 + 1;
            Burst_TIme = rand() % (12 - 2 + 1) + 2;
            printf("%d %d %d \n", i + 1, Arrival, Burst_TIme);
        }
    }
    return 0;
}