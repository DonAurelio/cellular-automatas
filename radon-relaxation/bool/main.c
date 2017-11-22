#include <stdlib.h>
#include <stdio.h>

#define ColDim 953 // Width
#define RowDim 668 // Height
// 636604 points
# define CaveZone 1	// Cave
# define Outside 0	// Exterior

/*
	Load a grayscale image in a pgm format. It asumes the image 
	0 as the zone we are interested and other values as not 
	interested zones. So it marks with 1 the interest zone and 
	0 the not interested zones. Then the resulting matrix has 
	just zeros and ones.
*/
void initialize(int * matrix)
{
	FILE * file = fopen("images/b-w.pgm","r");

	char buff[255];
	for (int i = 0; fscanf(file,"%s",buff) && !feof(file); ++i)
	{
		int value = atoi(buff);
		matrix[i] =  (value == 0) ? CaveZone : Outside;
	}

	fclose(file);
}

/* 
	Saves a bool matrix (zeros and ones) into a file called matrix.dat
	in a matrix format 
	
	000000
	110000
	111110
	001100
 	
 */ 
void save(int * matrix){
    FILE * pf = fopen("bool.dat","w");
    for (int i=0; i<RowDim; ++i){
        for (int j=0; j<ColDim; ++j) fprintf(pf, "%d\t", matrix[i * ColDim + j]);
        fprintf(pf, "\n");
    }
    fclose(pf);
}

int main(int argc, char const *argv[])
{
	int * matrix = (int *) malloc(RowDim*ColDim*sizeof(int));
	initialize(matrix);
	save(matrix);
	free(matrix);
	return 0;
}