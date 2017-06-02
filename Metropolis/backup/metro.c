#include <stdio.h>  /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Libary: malloc, calloc, free, ralloc functions */
#include <math.h>
#include <time.h>
#include <stdbool.h> /* C90 does not support boolean data type */
#include <omp.h>

#define pi 3.14159265358979323846
#define Nx 64
#define Ny 64
#define P 0.6
#define J 1.0
#define B 1.0
#define PositiveCharge 1
#define NegativeCharge -1

double random_eng(){  
    /* Reference: https://github.com/ArangoGutierrez/Montecarlo_RTE/blob/master/MC_RTE.cpp 
    random number Function Generator by using a linear congruential generator (LCG)
    is an algorithm that yields a sequence of pseudo-randomized numbers calculated
    with a discontinuous piecewise linear equation. */
    clock_t t;
    t=clock();
    double tc;
    tc = (((float)t)/CLOCKS_PER_SEC);
    long double X = fmod ((pi*t)+(pi/tc),(pi/tc));/*Seed*/
    double M = 2147483648,n;
    int a = 1103515245,c=12345,i;
    for(i=1;i<5;i++){
        X = fmod ((a*X+c),M);// Linear congruence
        n = (X/(M-1));
    }  
    return n;// Returns a number n with a [0,1) domain
}

void fill_matrix(int * matrix){
    int i = 0;
    for(i=0;i<Nx * Ny;++i) matrix[i] = ( random_eng() < P ) ? PositiveCharge : NegativeCharge;
}

void initialize_matrix(int * matrix){
    int i = 0;
    for(i=0; i<Nx*Ny;++i) matrix[i] = 0;
}

double check_probability(int * matrix){
    double sum = 0.0;
    int i = 0;
    for(i = 0; i < Nx*Ny; ++i) sum += (matrix[i] == 1) ? 1 : 0;
    return sum / (Nx*Ny);
}

void print_matriz(int * matrix){
    int i;
    for (i = 1; i <= Nx * Ny; i++){
        int b_i = (i-1)/Nx, b_j = (i-1)%Nx; 
        printf("%d\t",matrix[i-1]);
        if(i % Nx == 0) printf("\n");
    }
}

int random_site(){
    return (Nx * Ny) * random_eng();
}

double local_energy_bool(int * matrix, int site){
    int x = 0, y = 0;
    y = site / Ny, x = site % Nx;

    int up = ( y - 1 < 0 ) ? matrix[ site + (Nx * (Ny - 1))  ] : matrix[ site - Nx ];
    int down = ( y + 1 >= Ny ) ? matrix[ site - (Nx * (Ny - 1)) ] : matrix[ site + Nx ];
    int left = ( x - 1 < 0 ) ? matrix[ site + (Nx - 1) ] : matrix[ site - 1 ];
    int right = ( x + 1 >= Nx ) ? matrix[ site - (Nx - 1) ] : matrix[ site + 1 ];
    //printf("site : %d, x : %d, y : %d Up => %d\nDown => %d\nLeft => %d\nRight => %d\n",site,x,y,up,down,left,right);
    return -1.0 * J * matrix[site] * (up + down + left + right);
}

double total_energy(int * matrix){
    int x = 0, y = 0, i = 0;
    double sum = 0.0;
    for(i = 0; i < Nx*Ny; ++i){
        y = i / Ny, x = i % Nx;
        //int up = ( y - 1 < 0 ) ? matrix[ i + (Nx * (Ny - 1))  ] : matrix[ i - Nx ];
        int down = ( y + 1 >= Ny ) ? matrix[ i - (Nx * (Ny - 1)) ] : matrix[ i + Nx ];
        //int left = ( x - 1 < 0 ) ? matrix[ i + (Nx - 1) ] : matrix[ i - 1 ];
        int right = ( x + 1 >= Nx ) ? matrix[ i - (Nx - 1) ] : matrix[ i + 1 ];
        sum += matrix[i] * (down + right);
    }
    return -1.0 * J * sum;
}

void flip_site(int * matrix, int site){
    matrix[site] = ( matrix[ site ] == PositiveCharge ) ? NegativeCharge : PositiveCharge;
}

double transition_rate(double dE, double t){
    return exp( (-1.0 * dE) / (B*t) );
}

void metropolis_step(int * matrix, double t){
    int site = random_site();
    double E_old = local_energy_bool(matrix,site);
    flip_site(matrix,site);
    double E_new = local_energy_bool(matrix,site);

    double dE = E_new - E_old;
    if(dE >= 0){
        double r = random_eng();
        if(r >= transition_rate(dE,t)){
            flip_site(matrix,site);

        }
    }
 }

double magnetization(int * matrix) { //No arguments needed
    int aux = 0; //An auxiliar variable to add up the magnetization
    int i = 0;
    for (i = 0; i < (Nx*Ny); ++i)  aux += matrix[i]; //Magnetization
    return abs(aux) * 1.; //Total magnetization
}

int main(int argc, char ** argv){

    double t_init = ( argc >= 4 ) ? atof(argv[1]) : 0.5 ;
    double t_end = ( argc >= 4 ) ? atof(argv[2]) : 5.0 ;
    double t_step = ( argc >= 4 ) ? atof(argv[3]) : 0.1;
    int exp_id = ( argc >= 4 ) ? atof(argv[4]) : 0;
    
    FILE *output;
    char filename[50];

    sprintf(filename,"Exp_%d",exp_id);

    output = fopen(filename,"w+");
    int * matrix = (int *) malloc( Nx*Ny*sizeof(int));
    double t = 0.0; 
    for(t=t_init;t<=t_end;t=t+t_step){
        
        initialize_matrix(matrix);
        fill_matrix(matrix);
        double E_total = 0.0;
        double M = 0.0;

        int i = 0;
        for(i=0;i<5000000;++i)  metropolis_step(matrix,t);

        E_total = total_energy(matrix);
        M = magnetization(matrix);
        //printf("%f %f %f %f %f %f\n",t,E_total,M, pow(E_total,2), pow(M,2), pow(M,4) );
        fprintf(output,"%f %f %f %f %f %f\n",t,E_total,M, pow(E_total,2), pow(M,2), pow(M,4));

    }

    fclose(output);
    free(matrix); 

    
    return 0;
}