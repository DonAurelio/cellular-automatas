
/* -- Host libaries -- */
#include <stdio.h>  /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Library: malloc, calloc, free, ralloc */
#include <chrono>   /* Console Debug */
#include <thread>   /* Console Debug */
#include <math.h>
#include <time.h>
#include <omp.h>    /* OpenMP Header */    

#define MOD(a,b) ((((a)%(b))+(b))%(b))


void initialize( bool ** matrix, int rowDim, int colDim ){
    for (int i=0; i<rowDim; ++i){
        for (int j=0; j<colDim; ++j) matrix[i][j] = 0;
    }
}

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

void parallelstepGPT(bool ** in, bool ** out, int rowDim, int colDim){
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int num_threads = omp_get_num_threads();
        int blockDim = rowDim / (num_threads / 2);
        int thread_id = omp_get_thread_num();

        int blockx = thread_id / (num_threads/2);
        int blocky = MOD(thread_id,(num_threads/2));

        int x = 0;
        int y = 0;

        // if (thread_id == 1){
        //     printf("I ' am thread %d\n",thread_id);
        //     printf("BlockDim %d\n",blockDim);
        //     printf("blockx,blocky %d,%d\n",blockx,blocky);
        for(int i = 0; i < blockDim; ++i){
            x = blockDim * blockx + i;
            for(int j = 0; j < blockDim; ++j){
                y = blockDim * blocky + j;
                out[x][y] = nextState(in,x,y,rowDim,colDim);
                // printf("Point %d,%d\n",x,y);
            }
        }
        // }
    }
}

void savestatus(){
    FILE * pf = fopen("status.dat","w");
    fprintf(pf, "In parallel: %d\n", omp_in_parallel());
    fprintf(pf, "Number of therads running: %d\n", omp_get_num_threads());
    fprintf(pf, "Thead limit: %d\n", omp_get_thread_limit());
    fclose(pf); 
}

void parallestepFor(bool ** in, bool ** out, int rowDim, int colDim){
    int i = 0; 
    int j = 0;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #ifdef _OPENMP
        #pragma omp single
        {
            savestatus();
        }
        #endif
        #pragma omp for private(j)
        for (i = 0; i < rowDim; ++i){
            for (j = 0; j < colDim; ++j){
                out[i][j] = nextState(in,i,j,rowDim,colDim);
            }
        }
    }
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
        for (j=0; j<colDim; ++j){
            if(matrix[i][j] == 0)
                printf(" \t");
            else
                printf("%d\t",matrix[i][j]);
        } 
        printf("\n");
    }
    printf("\n");
}

void save(const char * filename, bool ** matrix, int rowDim, int colDim){
    FILE * pf = fopen(filename,"w");
    for (int i=0; i<rowDim; ++i){
        for (int j=0; j<colDim; ++j) fprintf(pf, "%d\t", matrix[i][j]);
        fprintf(pf, "\n");
    }
    fclose(pf);
}

void savetime(int matrixDim, float time){
    FILE * pf = fopen("time.dat","a+");
    fprintf(pf, "%d\t%f\n", matrixDim,time);
    fclose(pf); 
}



void evolve(bool ** in, bool ** out, int rowDim, int colDim, int generations){
    int i = 0;
    clock_t start = 0.0, end = 0.0;
    double sum = 0.0;
    char filename[30];

    // Save CA initial configuration
    #ifdef SAVEINIT
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,0);
    save(filename,in,rowDim,colDim);
    #endif

    for (i = 1; i <= generations; ++i){
       
        #if PARALLEL == 1
        start = clock();
        parallestepFor(in,out,rowDim,colDim);
        end = clock();
        #elif PARALLEL == 2
        start = clock();
        parallelstepGPT(in,out,rowDim,colDim);
        end = clock();
        #else
        start = 0.0;
        sequentialstep(in,out,rowDim,colDim);
        end = 0.0;
        #endif

        sum += (end -start) / (double) CLOCKS_PER_SEC;
        
        bool ** temp = in;
        in = out;
        out = temp;

        #ifdef DEBUG
        see(in,rowDim,colDim);
        std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
        printf("\033[H\033[J");
        #endif
        
        // Save all CA generations
        #ifdef SAVEALL
        sprintf(filename,"dim_%d_gen_%d.dat",rowDim,i);
        save(filename,in,rowDim,colDim);
        #endif 
    }

    // Save CA last generation
    #ifdef SAVELAST
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,generations);
    save(filename,in,rowDim,colDim);
    #endif

    #ifdef TIME 
    savetime(rowDim,sum);
    #endif

}

void parallelevolve(bool ** in, bool ** out, int rowDim, int colDim, int generations){
    int i = 0;
    clock_t start = 0.0, end = 0.0;
    double sum = 0.0;
    char filename[30];

    // Save CA initial configuration
    #ifdef SAVEINIT
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,0);
    save(filename,in,rowDim,colDim);
    #endif

    start = clock();
    #pragma omp parallel private(i) shared(in,out,rowDim,colDim,generations) num_threads(NUM_THREADS)
    {
        for (i = 1; i <= generations; ++i){
           
            int li = 0; 
            int lj = 0;
   
            #pragma omp for private(li,lj)
            for (li = 0; li < rowDim; ++li){
                for (lj = 0; lj < colDim; ++lj){
                    out[li][lj] = nextState(in,li,lj,rowDim,colDim);
                }
            }

            #pragma omp barrier 
            
            #pragma omp master
            {
                bool ** temp = in;
                in = out;
                out = temp;
            }

            #ifdef DEBUG
            #pragma omp master
            {
                see(in,rowDim,colDim);
                std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
                printf("\033[H\033[J");
            }
            #endif
            
            #pragma omp barrier

            // Save all CA generations
            // #ifdef SAVEALL
            // sprintf(filename,"dim_%d_gen_%d.dat",rowDim,i);
            // save(filename,in,rowDim,colDim);
            // #endif 
        }
    }
    end = clock();
    sum += (end -start) / (double) CLOCKS_PER_SEC;

    // Save CA last generation
    #ifdef SAVELAST
    sprintf(filename,"dim_%d_gen_%d.dat",rowDim,generations);
    save(filename,in,rowDim,colDim);
    #endif

    #ifdef TIME 
    savetime(rowDim,sum);
    #endif

}


int main(int argc, char const **argv)
{
    /* Matrix dimesions */
    /* The matrix dimensions has to be even (pair) and multiple of the number
    of processing cores */
    if(argc < 3){
        printf("CA dimension and number of generations are required as argument\n");
        return EXIT_FAILURE;
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

    initialize(in,rowDim,colDim);
    initialize(out,rowDim,colDim);
    fillCellularSpace(in,rowDim,colDim);
    evolve(in,out,rowDim,colDim,generations);
    // parallelevolve(in,out,rowDim,colDim,generations);

    /* -- Releasing resources -- */
    for (i=0; i<rowDim; ++i) free(in[i]);
    free(in);
    for (i=0; i<rowDim; ++i) free(out[i]);
    free(out);

    return EXIT_SUCCESS;
}
