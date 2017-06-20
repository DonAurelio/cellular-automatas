/*
References :https://www.lemoda.net/c/modulo-operator/
*/

#include <stdio.h>  /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Libary: malloc, calloc, free, ralloc functions */
#include <math.h> /* Random number generator */
#include <time.h> /* Random number generator */
#include <chrono>
#include <thread>

// #define Nx 4 /* did not work fill function */
// #define Ny 5
// #define Nx 12 /* did not work fill function */
// #define Ny 10
// #define Nx 11 /* did not work fill function */
// #define Ny 12 

/* It works properly only with square matrixes, I dont know why */
#define Nx 60
#define Ny 60
#define PI 3.14159265358979323846
#define MOD(a,b) ((((a)%(b))+(b))%(b))

double nrand()
{  
    /* Reference: https://github.com/ArangoGutierrez/Montecarlo_RTE/blob/master/MC_RTE.cpp 
    random number Function Generator by using a linear congruential generator (LCG)
    is an algorithm that yields a sequence of pseudo-randomized numbers calculated
    with a discontinuous piecewise linear equation. */
    clock_t t;
    t=clock();
    double tc;
    tc = (((float)t)/CLOCKS_PER_SEC);
    long double X = fmod ((PI*t)+(PI/tc),(PI/tc));/*Seed*/
    double M = 2147483648,n;
    int a = 1103515245,c=12345,i;
    for(i=1;i<5;i++){
        X = fmod ((a*X+c),M);// Linear congruence
        n = (X/(M-1));
    }  
    return n;// Returns a number n with a [0,1) domain
}

void fill(int ** matrix, double p)
{
    int i = 0, counter = 1;
    int j = 0;
    for (i=0; i<Ny; ++i){
        for (j=0; j<Nx; ++j) matrix[i][j] = ( nrand() < p) ? 1:0;
        // for (j=0; j<Nx; ++j){ 
        //     // printf("%d,%d\n",i,j);
        //     matrix[i][j] = counter;
        //     ++counter;
        // }
    }
}

void see(int ** matrix)
{
    printf("\n");
    int i = 0;
    for (i=0; i<Ny; ++i){
        int j = 0;
        // for (j=0; j<Nx; ++j) printf("%d ",matrix[i][j]);
        for (j=0; j<Nx; ++j) printf("%d\t",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
    
}

int periodic_transition(int ** matrix,int site_y, int site_x)
{
    int up = matrix[ MOD(site_y - 1,Ny) ][ site_x ];
    int down = matrix[ MOD(site_y + 1,Ny) ][ site_x ];
    
    int left = matrix[ site_y ][ MOD(site_x - 1,Nx) ];
    int left_up = matrix[ MOD(site_y - 1,Ny) ][ MOD(site_x - 1,Nx) ];
    int left_down = matrix[ MOD(site_y + 1,Ny) ][ MOD(site_x - 1,Nx) ];
    
    int right = matrix[ site_y ][ MOD(site_x + 1,Nx) ];
    int right_up = matrix[ MOD(site_y - 1,Ny) ][ MOD(site_x + 1,Nx) ];
    int right_down = matrix[ MOD(site_y + 1,Ny) ][ MOD(site_x + 1,Nx) ];

    // Hexagonal Grid
    // int sum = ((site_x + 1 % 2) == 0) ? up + left + right + left_down + down + right_down : left_up + up + right_up + left + right + down;
    // Normal grid
    int sum = left_up + up + right_up + left + right + left_down + down + right_down;
    int site = matrix[site_y][site_x];
    int life = ( site == 1 ) ? ( sum == 2 || sum == 3 ) ? 1:0 : ( sum == 3 ) ? 1:0;

    // printf("%d\t%d\t%d\n%d\t%d\t%d\n%d\t%d\t%d\n",
    //     left_up,up,right_up,
    //     left,matrix[site_y][site_x],right,
    //     left_down,down,right_down);
    // printf("\n");

    return life;
}

void step(int ** in, int ** out)
{
    int i = 0, j = 0;
    for (i = 0; i < Ny; ++i){
        for (j = 0; j < Nx; ++j){
            out[i][j] = periodic_transition(in,i,j);
        }
    }
}



int main(int argc, char const **argv)
{
    int ** in = (int **) malloc(Ny*sizeof(int*));
    int ** out = (int **) malloc(Ny*sizeof(int*));
    int i = 0;
    for (i=0; i<Ny; ++i){ 
        in[i] = (int *) malloc(Nx*sizeof(int));
        out[i] = (int *) malloc(Nx*sizeof(int));
    }

    fill(in,0.05);
    fill(out,0.0);
    // see(in);
    // see(out);


    /* Evolve */
    for (i = 1; i <= 100; ++i){
        printf("Generation %d\n",i);
        step(in,out);
        see(out);

        int ** temp = in;
        in = out;
        out = temp;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
        printf("\033[H\033[J");
    }



    for (i=0; i<Nx; ++i) free(in[i]);
    free(in);
    for (i=0; i<Nx; ++i) free(out[i]);
    free(out);
    return 0;
}

/*
    Compile
     g++ ca.c -lm -std=gnu++11

*/
