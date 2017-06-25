#include <stdlib.h>
#include <stdio.h>


struct cell
{
    int value;
};

int main(int argc, char const **argv)
{
    int size = 20480000;
    struct cell * vector = (struct cell*) malloc(sizeof(*vector)*size);

    #pragma acc kernels
    {
        for (int i=0; i<size; ++i) vector[i].value = i+1 ;
    }

    for (int i=0; i<100; ++i) printf("%d\n",vector[i].value);

    free(vector);

    return 0;
}
