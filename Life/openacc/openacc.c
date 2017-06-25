#include <stdio.h>  /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono>   /* Console Debug */
#include <thread>   /* Console Debug */
#include <math.h>
#include <time.h>

#define MOD(a,b) ((((a)%(b))+(b))%(b))

void initialize( bool ** matrix){
    for (int i=0; i<SIZE; ++i){
        for (int j=0; j<SIZE; ++j) matrix[i][j] = 0;
    }
}

void fillCellularSpace( bool ** matrix){
    matrix[10][10] = 1;
    matrix[10][11] = 1;
    matrix[11][11] = 1;
    matrix[10][12] = 1;
}

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

struct Neighborhood neighborhoodOf(bool ** matrix, int row, int col){
    struct Neighborhood nbhd;

    nbhd.up = matrix[ MOD(row - 1,SIZE) ][ col ];
    nbhd.down = matrix[ MOD(row + 1,SIZE) ][ col ];
    nbhd.center = matrix[row][col];
    
    nbhd.left = matrix[ row ][ MOD(col - 1,SIZE) ];
    nbhd.left_up = matrix[ MOD(row - 1,SIZE) ][ MOD(col - 1,SIZE) ];
    nbhd.left_down = matrix[ MOD(row + 1,SIZE) ][ MOD(col - 1,SIZE) ];
    
    nbhd.right = matrix[ row ][ MOD(col + 1,SIZE) ];
    nbhd.right_up = matrix[ MOD(row - 1,SIZE) ][ MOD(col + 1,SIZE) ];
    nbhd.right_down = matrix[ MOD(row + 1,SIZE) ][ MOD(col + 1,SIZE) ];


    // printf("Neighborhood of %d,%d\n", row, col);
    // printf("%d\t%d\t%d\n%d\t%d\t%d\n%d\t%d\t%d\n",
    //     nbhd.left_up,nbhd.up,nbhd.right_up,
    //     nbhd.left,nbhd.center,nbhd.right,
    //     nbhd.left_down,nbhd.down,nbhd.right_down);

    return nbhd;
}


bool nextState(bool ** matrix, int row, int col){
    
    struct Neighborhood nbhd = neighborhoodOf(matrix,row,col);

    int sum = nbhd.left_up + nbhd.up + nbhd.right_up + nbhd.left + nbhd.right + \
    nbhd.left_down + nbhd.down + nbhd.right_down;
    int site = nbhd.center;
    int life = ( site == 1 ) ? ( sum == 2 || sum == 3 ) ? 1:0 : ( sum == 3 ) ? 1:0;

 //    int life = nbhd.center;
 //    // A live cell will die if it has less than two live neighbours (under population)
 //    // A live cell will die if it has more than three live neighbours (overcrowding)
 //    if(life == 1 && (sum < 2 || sum > 3)){
 //    	life = 0;
 //    }else if(life == 1 && (sum == 2 || sum == 3)){
 //    // A live cell will stay alive in the next generation if it has two or three live neighbours
 //    	life = 1;
 //    }else if(life == 0 && sum == 3){
	// // A dead cell will become alive if it has exactly three live neighbours. Else it will stay dead
 //    	life = 1;
 //    }

    return life;
}

void see(bool ** matrix){
    printf("\n");
    int i = 0;
    for (i=0; i<SIZE; ++i){
        int j = 0;
        for (j=0; j<SIZE; ++j){
            if(matrix[i][j] == 0)
                printf(" \t");
            else
                printf("%d\t",matrix[i][j]);
        } 
        printf("\n");
    }
    printf("\n");
}

void save(const char * filename, bool ** matrix){
    FILE * pf = fopen(filename,"w");
    for (int i=0; i<SIZE; ++i){
        for (int j=0; j<SIZE; ++j) fprintf(pf, "%d\t", matrix[i][j]);
        fprintf(pf, "\n");
    }
    fclose(pf);
}

void savetime(float time){
    FILE * pf = fopen("time.dat","a+");
    fprintf(pf, "%d\t%f\n", SIZE,time);
    fclose(pf); 
}

void evolve(bool ** in, bool ** out){
    clock_t start = 0.0, end = 0.0;
    double sum = 0.0;
    char filename[30];

    int rowDim = SIZE;
    int colDim = SIZE;
    int generations = GENERATIONS;

    // Save CA initial configuration
    #ifdef SAVEINIT
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,0);
    save(filename,in);
    #endif

    bool in_new[SIZE][SIZE];

    start = clock();
    #pragma acc data copy(in[0:rowDim][0:colDim]), create(in_new)
    {
        for (int i = 1; i <= generations; ++i){
            #pragma acc parallel loop vector_length(colDim) num_gangs(rowDim) gang 
            for (int i = 0; i < rowDim; ++i){
                #pragma acc loop vector
                for (int j = 0; j < colDim; ++j){
                    in_new[i][j] = nextState(in,i,j);
                }
            }

            #pragma acc parallel loop
            for (int i = 0; i < rowDim; ++i){
                #pragma acc loop
                for (int j = 0; j < colDim; ++j){
                    in[i][j] = in_new[i][j];
                }
            }
        }
    }
    end = clock();
    sum += (end -start) / (double) CLOCKS_PER_SEC;

    // Save CA last generation
    #ifdef SAVELAST
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,generations);
    save(filename,in);
    #endif

    #ifdef TIME 
    savetime(sum);
    #endif

}

int main(int argc, char const **argv)
{

    #ifndef SIZE
    printf("CA dimension is required as compiler argument use -D SIZE=<int> as compilation flag\n");
    #endif

    #ifndef GENERATIONS
    printf("CA generations is required as compiler argument use -D GENERATIONS=<int> as compilation flag\n");
    #endif
    
    int rowDim = SIZE, colDim = SIZE;

    /* -- Celular space initialization -- */
    //calloc intPtr = (int *) calloc (sizeof (int), 1000);
    //dataPtr = (struct dataEntry *) calloc (n, sizeof (struct dataEntry));
    bool ** in = (bool **) malloc(rowDim*sizeof( bool *));
    bool ** out = (bool **) malloc(rowDim*sizeof( bool *));
    int i = 0;
    for (i=0; i<rowDim; ++i){ 
        in[i] = (bool *) malloc(colDim*sizeof(bool));
        out[i] = (bool *) malloc(colDim*sizeof(bool));
    }

    initialize(in);
    initialize(out);
    fillCellularSpace(in);
    evolve(in,out);

    /* -- Releasing resources -- */
    for (i=0; i<rowDim; ++i) free(in[i]);
    free(in);
    for (i=0; i<rowDim; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
