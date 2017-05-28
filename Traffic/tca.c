
/* -- Host libaries -- */
#include <stdio.h> /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono> /* Console Debug */
#include <thread> /* Console Debug */
#include <math.h> /* Random Number Generator */
#include <time.h> /* Random Number Generator */

#define colDim 6
#define rowDim 50
#define Vmax 5
#define Freeway -1
#define DeaccelProb 0.0
#define CarProb 0.2
#define Generations 50
#define PI 3.14159265358979323846
#define MOD(a,b) ((((a)%(b))+(b))%(b))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

double rnd(){
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

    return n;
}

int initialize( int row, int col ){
    return ( rnd() < CarProb && (col == colDim/2 || col == colDim/2 - 1 || col == colDim/2 + 1) ) \
     ? rnd() * Vmax + 1: Freeway; 
}

void fillCellularSpace( int ** matrix ){
    for (int i = 0; i < rowDim; ++i){
        for (int j = 0; j < colDim; ++j){
            matrix[i][j] = initialize(i,j);
        }
    }
}

void clearCellularSpace( int ** matrix ){
    for (int i = 0; i < rowDim; ++i){
        for (int j = 0; j < colDim; ++j){
            matrix[i][j] = Freeway;
        }
    } 
}

struct Neighborhood
{
    int up;
    int center;
    int down;
    int left;
    int left_up;
    int left_down;
    int right;
    int right_up;
    int right_down;

};

struct Neighborhood neighborhoodOf( int ** matrix, int row, int col){
    struct Neighborhood neighborhood;
    neighborhood.up = matrix[ MOD(row - 1,rowDim) ][ col ];
    neighborhood.down = matrix[ MOD(row + 1,rowDim) ][ col ];
    
    neighborhood.left = matrix[ row ][ MOD(col - 1,colDim) ];
    neighborhood.left_up = matrix[ MOD(row - 1,rowDim) ][ MOD(col - 1,colDim) ];
    neighborhood.left_down = matrix[ MOD(row + 1,rowDim) ][ MOD(col - 1,colDim) ];
    
    neighborhood.right = matrix[ row ][ MOD(col + 1,colDim) ];
    neighborhood.right_up = matrix[ MOD(row - 1,rowDim) ][ MOD(col + 1,colDim) ];
    neighborhood.right_down = matrix[ MOD(row + 1,rowDim) ][ MOD(col + 1,colDim) ];

    return neighborhood;
}


void nextState( int ** in, int ** out, int row, int col ){
    
    // struct Neighborhood nbhd = neighborhoodOf(in,col,row);

    int velocity = in[row][col];

    if(velocity != Freeway){
        // Step 1: Acceleration
        velocity = MIN(velocity + 1,Vmax);
        // Step 2: Safe distance
        int freecells = 0;
        for(int i = row; in[ MOD( i - 1 , rowDim ) ][col] == Freeway; i = MOD( i - 1, rowDim )) ++freecells;
        velocity = MIN(velocity,freecells - 1);
        // Step 3: Randomness deacceleration
        velocity = ( rnd() < DeaccelProb ) ? MAX(0,velocity - 1) : velocity;
        // Step 4: New position
        out[ MOD( row - velocity, rowDim ) ][col] = velocity;
    }
}

void step( int ** in, int ** out)
{
    int i = 0, j = 0;
    for (i = 0; i < rowDim; ++i){
        for (j = 0; j < colDim; ++j){
            nextState(in,out,i,j);
        }
    }
}

void see( int ** matrix){
    printf("\n");
    int i = 0;
    for (i=0; i<rowDim; ++i){
        int j = 0;
        for (j=0; j<colDim; ++j){
            if(matrix[i][j] == Freeway)
                printf("[ ] ");
            else
                // printf("[%d] ",matrix[i][j]);
                printf("[8] ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void evolve( int ** in, int ** out){
    int i = 0;
    for (i = 1; i <= Generations; ++i){
        step(in,out);
        clearCellularSpace(in);
        see(out);
        int ** temp = in;
        in = out;
        out = temp;

        std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
        printf("\033[H\033[J");
        
    }
}

int main(int argc, char const **argv)
{
    /* -- Celular space initialization -- */
    int ** in = ( int **) malloc(rowDim*sizeof( int* ));
    int ** out = ( int **) malloc(rowDim*sizeof( int* ));

    // if( in == NULL || out == NULL ){
    //     printf("Memoria insuficiente\n");
    //     exit(EXIT_FAILURE);
    // }

    /* rows allocation */
    for (int i=0; i<rowDim; ++i){ 
        /* for each row we allocate colDim integers */
        in[i] = ( int *) malloc(colDim*sizeof(int));
        out[i] = ( int *) malloc(colDim*sizeof(int));
    }

    fillCellularSpace(in);
    clearCellularSpace(out);
    // see(in);

    /* Running simulation */
    evolve(in,out);

    /* -- Releasing resources -- */
    for (int i=0; i<rowDim; ++i) free(in[i]);
    free(in);
    for (int i=0; i<rowDim; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
