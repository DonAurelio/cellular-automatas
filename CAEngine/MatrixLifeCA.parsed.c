
/* -- Host libaries -- */
#include <stdio.h> /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono> /* Console Debug */
#include <thread> /* Console Debug */

#include <math.h>

#include <time.h>



#define Dx 10
#define Dy 10

#define PI 3.14159265358979323846

#define MOD(a,b) ((((a)%(b))+(b))%(b))

/* -- Initialize CA space: functions definition -- */
bool initialize( int xcoor, int ycoor ){
    
    double pi = 3.14159265358979323846;
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
    return (n < 1) ? 1 : 0;

}
/* The user defines how initialice the cellular space */
void fillCellularSpace( bool ** matrix ){
    int i = 0, j = 0;
    for (i = 0; i < Dy; ++i){
        for (j = 0; j < Dx; ++j){
            matrix[i][j] = initialize(i,j);
        }
    }
}

/* -- Neighbooring function definition */

struct Neighborhood
{
    bool up;
    bool center;
    bool down;
    bool left;
    bool left_up;
    bool left_down;
    bool right;
    bool right_up;
    bool right_down;

};

/*
This function has to return the neighborhood of a given site on the cellular space
*/
struct Neighborhood neighborhoodOf( bool ** matrix, int xcoor, int ycoor){
    struct Neighborhood neighborhood;
    neighborhood.up = matrix[ MOD(ycoor - 1,Dy) ][ xcoor ];
    neighborhood.down = matrix[ MOD(ycoor + 1,Dy) ][ xcoor ];
    
    neighborhood.left = matrix[ ycoor ][ MOD(xcoor - 1,Dx) ];
    neighborhood.left_up = matrix[ MOD(ycoor - 1,Dy) ][ MOD(xcoor - 1,Dx) ];
    neighborhood.left_down = matrix[ MOD(ycoor + 1,Dy) ][ MOD(xcoor - 1,Dx) ];
    
    neighborhood.right = matrix[ ycoor ][ MOD(xcoor + 1,Dx) ];
    neighborhood.right_up = matrix[ MOD(ycoor - 1,Dy) ][ MOD(xcoor + 1,Dx) ];
    neighborhood.right_down = matrix[ MOD(ycoor + 1,Dy) ][ MOD(xcoor + 1,Dx) ];

    return neighborhood;
}


/* -- Transition function definition -- */

/*
This function has to define how a given site change its state to an new one
*/
bool nextState( bool ** matrix, int xcoor, int ycoor ){
    
    struct Neighborhood nbhd = neighborhoodOf(matrix,xcoor,ycoor);

    int sum = nbhd.left_up + nbhd.up + nbhd.right_up + nbhd.left + nbhd.right + nbhd.left_down + nbhd.down + nbhd.right_down;
    int site = nbhd.center;
    int life = ( site == 1 ) ? ( sum == 2 || sum == 3 ) ? 1:0 : ( sum == 3 ) ? 1:0;

    return life;

}

/*
This function brings the cellular space to the next generation
HINT : THE BODY OF THIS FUNCTION IS ATRACTIVE FOR PARALLEL EXECUTION
*/
void step( bool ** in, bool ** out)
{
    int i = 0, j = 0;
    for (i = 0; i < Dy; ++i){
        for (j = 0; j < Dx; ++j){
            out[i][j] = nextState(in,i,j);
        }
    }
}

/*
Visualization function
*/
void see( bool ** matrix){
    printf("\n");
    int i = 0;
    for (i=0; i<Dy; ++i){
        int j = 0;
        for (j=0; j<Dx; ++j) printf("%d\t",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

/*
This function aims for the way in wich the transition rule (next state) is applyed 
into the cellular space 
*/
void evolve( bool ** in, bool ** out){
    int i = 0;
    for (i = 1; i <= 12; ++i){
        step(in,out);
        if ( 1 ) see(out);
        bool ** temp = in;
        in = out;
        out = temp;

        std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
        printf("\033[H\033[J");
    }
}

int main(int argc, char const **argv)
{
    /* -- Celular space initialization -- */
    //calloc intPtr = (int *) calloc (sizeof (int), 1000);
    //dataPtr = (struct dataEntry *) calloc (n, sizeof (struct dataEntry));
    bool ** in = ( bool **) malloc(Dy*sizeof( bool *));
    bool ** out = ( bool **) malloc(Dy*sizeof( bool *));
    int i = 0;
    for (i=0; i<Dx; ++i){ 
        in[i] = ( bool *) malloc(Dx*sizeof(int));
        out[i] = ( bool *) malloc(Dx*sizeof(int));
    }
   
    fillCellularSpace(in);

    /* Some times is neccessary some way of visualice the cellular space */
    // show or see function
    see(in);

    /* Running simulation */
    evolve(in,out);

    /* -- Releasing resources -- */
    for (i=0; i<Dx; ++i) free(in[i]);
    free(in);
    for (i=0; i<Dx; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
