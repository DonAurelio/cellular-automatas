#include <stdio.h>  /* Standard I/O Library: printf */
#include <stdlib.h> /* Standard Libary: malloc, calloc, free, ralloc functions */

void print_matriz(double * matrix, int Nx, int Ny){
    int i;
    for (i = 1; i <= Nx * Ny; ++i){
        printf("%lf ",matrix[i-1]);
        if( i % Nx == 0) printf("\n");
     }
}

int main(int argc, char const ** argv)
{
    int t_rows = ( argc >= 2 ) ? atof(argv[1]) : 0 ; // Number of temperatures
    int n_exp = ( argc >= 2 ) ? atof(argv[2]) : 0 ; // Number of experiments
    int offset = 6; // Number of columns on the table T, E_total, M, E_total^2, M^2, M^4
    
    double * matrix = (double *) malloc( t_rows*offset*sizeof(double));
    int i = 0;
    for(i=0;i<t_rows*offset;++i) matrix[i] = 0.0;


    /* Summatory */
    int exp_id = 0;
    char filename[50];
    for( exp_id = 0; exp_id < n_exp ;++exp_id ){
        sprintf(filename,"Exp_%d",exp_id);
        FILE* file = fopen(filename, "r");
        
        int row = 0; double t = 0.0, e = 0.0, m = 0.0, e2 = 0.0, m2 = 0.0, m4 = 0.0;
        
        for(; fscanf(file,"%lf %lf %lf %lf %lf %lf", &t, &e, &m, &e2, &m2, &m4) && !feof(file);){
           //printf("%s => %lf %lf %lf %lf %lf %lf\n",filename,t, e, m, e2, m2, m4);
            matrix[ ( row * offset) + 0 ] += t;
            matrix[ ( row * offset) + 1 ] += e;
            matrix[ ( row * offset) + 2 ] += m;
            matrix[ ( row * offset) + 3 ] += e2;
            matrix[ ( row * offset) + 4 ] += m2;
            matrix[ ( row * offset) + 5 ] += m4;
            ++row;
        }
        fclose(file);
    }

    /* Average */
    for(i=0;i<t_rows*offset;++i) matrix[i] = matrix[i] / n_exp;

    print_matriz(matrix,offset,t_rows);
    free(matrix);

    return 0;
}