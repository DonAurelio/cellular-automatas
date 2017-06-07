
/* -- Host libaries -- */
#include <stdio.h> /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono> /* Console Debug */
#include <thread> /* Console Debug */
#include <math.h>
#include <time.h>

#define MOD(a,b) ((((a)%(b))+(b))%(b))

void fillCellularSpace( bool ** matrix, int rowDim, int colDim ){
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

struct Neighborhood neighborhoodOf(bool ** matrix, int row, int col, int rowDim, int colDim){
    struct Neighborhood nbhd;
    nbhd.up = matrix[ MOD(row - 1,rowDim) ][ col ];
    nbhd.down = matrix[ MOD(row + 1,rowDim) ][ col ];
    nbhd.center = matrix[row][col];
    
    nbhd.left = matrix[ row ][ MOD(col - 1,colDim) ];
    nbhd.left_up = matrix[ MOD(row - 1,rowDim) ][ MOD(col - 1,colDim) ];
    nbhd.left_down = matrix[ MOD(row + 1,rowDim) ][ MOD(col - 1,colDim) ];
    
    nbhd.right = matrix[ row ][ MOD(col + 1,colDim) ];
    nbhd.right_up = matrix[ MOD(row - 1,rowDim) ][ MOD(col + 1,colDim) ];
    nbhd.right_down = matrix[ MOD(row + 1,rowDim) ][ MOD(col + 1,colDim) ];

    // printf("Neighborhood of %d,%d\n", row, col);
    // printf("%d\t%d\t%d\n%d\t%d\t%d\n%d\t%d\t%d\n",
    //     nbhd.left_up,nbhd.up,nbhd.right_up,
    //     nbhd.left,nbhd.center,nbhd.right,
    //     nbhd.left_down,nbhd.down,nbhd.right_down);

    return nbhd;
}


bool nextState(bool ** matrix, int row, int col, int rowDim, int colDim){
    
    struct Neighborhood nbhd = neighborhoodOf(matrix,row,col,rowDim,colDim);

    int sum = nbhd.left_up + nbhd.up + nbhd.right_up + nbhd.left + nbhd.right + nbhd.left_down + nbhd.down + nbhd.right_down;
    // int site = nbhd.center;
    // int life = ( site == 1 ) ? ( sum == 2 || sum == 3 ) ? 1:0 : ( sum == 3 ) ? 1:0;

    int life = nbhd.center;
    // A live cell will die if it has less than two live neighbours (under population)
    // A live cell will die if it has more than three live neighbours (overcrowding)
    if(life == 1 && (sum < 2 || sum > 3)){
    	life = 0;
    }else if(life == 1 && (sum == 2 || sum == 3)){
    // A live cell will stay alive in the next generation if it has two or three live neighbours
    	life = 1;
    }else if(life == 0 && sum == 3){
	// A dead cell will become alive if it has exactly three live neighbours. Else it will stay dead
    	life = 1;
    }

    return life;

}

void sequentialstep(bool ** in, bool ** out, int rowDim, int colDim){
    int i = 0, j = 0;
    for (i = 0; i < rowDim; ++i){
        for (j = 0; j < colDim; ++j){
            out[i][j] = nextState(in,i,j,rowDim,colDim);
        }
    }
}

void see(bool ** matrix, int rowDim, int colDim){
    printf("\n");
    int i = 0;
    for (i=0; i<rowDim; ++i){
        int j = 0;
        for (j=0; j<colDim; ++j) printf("%d\t",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void evolve(bool ** in, bool ** out, int rowDim, int colDim, int generations){
    int i = 0;
    clock_t start = 0.0, end = 0.0;
    double sum = 0.0;
    for (i = 1; i <= generations; ++i){
        start = clock();
        sequentialstep(in,out,rowDim,colDim);
        end = clock();
        sum += (end -start) / (double) CLOCKS_PER_SEC;
        
        #ifdef DEBUG    
        see(out,rowDim,colDim);
        #endif

        bool ** temp = in;
        in = out;
        out = temp;
        
        #ifdef DEBUG
        std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
        printf("\033[H\033[J");
        #endif 
    }
    // Average time calculation per generation
    // Number of generations per second
    #ifndef DEBUG
    printf("%f\t%f\t%f\n",sum,(sum/generations),generations/sum);
    #endif
}

int main(int argc, char const **argv)
{
    /* Matrix dimesions */
    /* The matrix dimensions has to be even (pair) and multiple of the number
    of processing cores */
    if(argc < 3){
        printf("CA dimension and number of generations are required as argument\n");
        return EXIT_SUCCESS;
    }
    
    int dim = atoi(argv[1]);
    int generations = atoi(argv[2]);
    int rowDim = dim, colDim = dim;

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

    fillCellularSpace(in,rowDim,colDim);
    evolve(in,out,rowDim,colDim,generations);

    /* -- Releasing resources -- */
    for (i=0; i<rowDim; ++i) free(in[i]);
    free(in);
    for (i=0; i<rowDim; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
