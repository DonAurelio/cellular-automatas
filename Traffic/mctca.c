
/* -- Host libaries -- */
#include <stdio.h> /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono> /* Console Debug */
#include <thread> /* Console Debug */
#include <math.h> /* Random Number Generator */
#include <time.h> /* Random Number Generator */

#define colDim 9
#define rowDim 50

#define Freeway 0
#define Car 1
#define Motorbike 2

#define VehicheProb 0.1
#define CarProb 0.1

#define Vmax 5
#define DeaccelProb 0.0
#define Generations 50
#define PI 3.14159265358979323846
#define MOD(a,b) ((((a)%(b))+(b))%(b))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


struct Cell
{
    int type;
    int velocity;
    bool evaluated;

};

// double rnd(){
//     clock_t t;
//     t=clock();
//     double tc;
//     tc = (((float)t)/CLOCKS_PER_SEC);
//     long double X = fmod ((PI*t)+(PI/tc),(PI/tc));/*Seed*/
//     double M = 2147483648,n;
//     int a = 1103515245,c=12345,i;
//     for(i=1;i<5;i++){
//         X = fmod ((a*X+c),M);// Linear congruence
//         n = (X/(M-1));
//     }  

//     return n;
// }

// void initialize( struct Cell ** matrix ){
//     for (int i = 0; i < rowDim; ++i){
//         for (int j = 0; j < colDim; ++j){
//             matrix[i][j] = (struct Cell) { .type = Freeway, .velocity = 0 };
//         }
//     } 
// }

// void newCar(struct Cell ** matrix, int row, int col){
//     if(row + 1 < rowDim && col + 1 < colDim){
//         struct Cell car = { .type = Car, .velocity = rnd() * Vmax + 1 };
//         matrix[row][col] = car;
//         matrix[row][col + 1] = car;
//         matrix[row + 1][col] = car;
//         matrix[row + 1][col + 1] = car;
//     }
// }

// void newMotorbike(struct Cell ** matrix, int row, int col){
//     if(row + 1 < rowDim && col + 1 < colDim){
//         struct Cell motorbike = { .type = Motorbike, .velocity = rnd() * Vmax + 1 };
//         matrix[row][col] = motorbike;
//     }
// }

// void fillCellularSpace( struct Cell ** matrix ){
//     for (int i = 0; i < rowDim; ++i){
//         for (int j = 0; j < colDim; ++j){
//             if(rnd() < VehicheProb && (matrix[i][j]).type == Freeway){
//                 if(rnd() < CarProb)
//                     newCar(matrix,i,j);
//                 else
//                     newMotorbike(matrix,i,j);
//             }
//         }
//     }
// }


// void nextState( struct Cell ** in, struct Cell ** out, int row, int col ){

//     struct Cell cell = in[row][col];

//     if(cell.type != Freeway){
//         // Step 1: Acceleration
//         velocity = MIN(velocity + 1,Vmax);
//         // Step 2: Safe distance
//         int freecells = 0;
//         for(int i = row; in[ MOD( i - 1 , rowDim ) ][col] == Freeway; i = MOD( i - 1, rowDim )) ++freecells;
//         velocity = MIN(velocity,freecells - 1);
//         // Step 3: Randomness deacceleration
//         velocity = ( rnd() < DeaccelProb ) ? MAX(0,velocity - 1) : velocity;
//         // Step 4: New position
//         out[ MOD( row - velocity, rowDim ) ][col] = velocity;
//     }
// }

// void step( int ** in, int ** out)
// {
//     int i = 0, j = 0;
//     for (i = 0; i < rowDim; ++i){
//         for (j = 0; j < colDim; ++j){
//             nextState(in,out,i,j);
//         }
//     }
// }

// void see(struct Cell ** matrix){
//     printf("\n");
//     int i = 0;
//     for (i=0; i<rowDim; ++i){
//         int j = 0;
//         for (j=0; j<colDim; ++j){
//             if(matrix[i][j].type == Freeway)
//                 printf("[ ] ");
//             else{
//                 if(matrix[i][j].type == Car)
//                     printf("[*] ");
//                 else
//                     printf("[^] ");
//             }
//         }
//         printf("\n");
//     }
//     printf("\n");
// }

// void evolve( int ** in, int ** out){
//     int i = 0;
//     for (i = 1; i <= Generations; ++i){
//         step(in,out);
//         clearCellularSpace(in);
//         see(out);
//         int ** temp = in;
//         in = out;
//         out = temp;

//         std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
//         printf("\033[H\033[J");
        
//     }
// }

int main(int argc, char const **argv)
{
    /* Celular space initialization */
    struct Cell *** in = ( struct Cell *** ) malloc(rowDim*sizeof( struct Cell ** ));
    struct Cell *** out = ( struct Cell *** ) malloc(rowDim*sizeof( struct Cell ** ));

    /* Allocation error checking */
    if( in == NULL || out == NULL ){
        printf("Memoria insuficiente\n");
        exit(EXIT_FAILURE);
    }

    /* Rows allocation */
    for (int i=0; i<rowDim; ++i){ 
        /* for each row we allocate colDim integers */
        in[i] = ( struct Cell ** ) malloc(colDim*sizeof( struct Cell * ));
        out[i] = ( struct Cell ** ) malloc(colDim*sizeof( struct Cell * ));
    }

    for (int i=0; i<rowDim; ++i){
        for(int j=0; j<colDim; ++j){
            in[i][j] = ( struct Cell * ) malloc(sizeof( struct Cell ));
            out[i][j] = ( struct Cell * ) malloc(sizeof( struct Cell ));
            
            struct Cell cellin = (struct Cell) { .type = Freeway, .velocity = 0 };
            in[i][j] = NULL;
            
            struct Cell cellout = (struct Cell) { .type = Freeway, .velocity = 0 };
            out[i][j] = NULL;
        }
    }

    /* -- Releasing resources -- */
    for (int i=0; i<rowDim; ++i){
        for(int j=0; j<colDim; ++j){
            free(in[i][j]);
            free(out[i][j]);
        }
    }

    for (int i=0; i<rowDim; ++i) free(in[i]);
    free(in);

    for (int i=0; i<rowDim; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
