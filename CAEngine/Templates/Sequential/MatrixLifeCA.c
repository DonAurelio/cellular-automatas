{% autoescape off %}
/* -- Host libaries -- */
#include <stdio.h> /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono> /* Console Debug */
#include <thread> /* Console Debug */
{% for library in libraries %}
#include <{{ library }}>
{% endfor %}


#define Dx {{ lattice.size.x }}
#define Dy {{ lattice.size.y }}
{% for constant, value in constants %}
#define {{ constant }} {{ value }}
{% endfor %}
#define MOD(a,b) ((((a)%(b))+(b))%(b))

/* -- Initialize CA space: functions definition -- */
{{ lattice.states.type }} initialize( int xcoor, int ycoor ){
    {{ lattice.states.initialize }}
}
/* The user defines how initialice the cellular space */
void fillCellularSpace( {{ lattice.states.type }} ** matrix ){
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
    {{ lattice.states.type }} up;
    {{ lattice.states.type }} center;
    {{ lattice.states.type }} down;
    {{ lattice.states.type }} left;
    {{ lattice.states.type }} left_up;
    {{ lattice.states.type }} left_down;
    {{ lattice.states.type }} right;
    {{ lattice.states.type }} right_up;
    {{ lattice.states.type }} right_down;

};

/*
This function has to return the neighborhood of a given site on the cellular space
*/
struct Neighborhood neighborhoodOf( {{ lattice.states.type }} ** matrix, int xcoor, int ycoor){
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
{{ lattice.states.type }} nextState( {{ lattice.states.type }} ** matrix, int xcoor, int ycoor ){
    {{ lattice.states.transition }}
}

/*
This function brings the cellular space to the next generation
HINT : THE BODY OF THIS FUNCTION IS ATRACTIVE FOR PARALLEL EXECUTION
*/
void step( {{ lattice.states.type }} ** in, {{ lattice.states.type }} ** out)
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
void see( {{ lattice.states.type }} ** matrix){
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
void evolve( {{ lattice.states.type }} ** in, {{ lattice.states.type }} ** out){
    int i = 0;
    for (i = 1; i <= {{ run.generations }}; ++i){
        step(in,out);
        if ( {{ run.debug }} ) see(out);
        {{ lattice.states.type }} ** temp = in;
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
    {{ lattice.states.type }} ** in = ( {{ lattice.states.type }} **) malloc(Dy*sizeof( {{ lattice.states.type }} *));
    {{ lattice.states.type }} ** out = ( {{ lattice.states.type }} **) malloc(Dy*sizeof( {{ lattice.states.type }} *));
    int i = 0;
    for (i=0; i<Dx; ++i){ 
        in[i] = ( {{ lattice.states.type }} *) malloc(Dx*sizeof(int));
        out[i] = ( {{ lattice.states.type }} *) malloc(Dx*sizeof(int));
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
{% endautoescape %}