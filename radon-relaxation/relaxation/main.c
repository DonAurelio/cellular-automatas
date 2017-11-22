#include <stdlib.h>
#include <stdio.h>

#define ColDim 953 // Width
#define RowDim 668 // Height
// 636604 points

#define HeatPointsDim 6

struct HeatPoint
{
	int x;
	int y;
	int heat;
};

float average_temperature(struct HeatPoint * heat_points)
{
	float sum = 0.0;
	for (int i = 0; i < HeatPointsDim; ++i) sum = sum + heat_points[i].heat;
	printf("Average Temperature %f\n", sum / HeatPointsDim );
	return sum / HeatPointsDim;
}

void initialize_bool(int * matrix)
{
	FILE * file = fopen("../bool/bool.dat","r");

	char buff[255];
	for (int i = 0; fscanf(file,"%s",buff) && !feof(file); ++i)
	{
		int value = atoi(buff);
		matrix[i] = value;
	}

	fclose(file);
}

void initialize_temperatures(struct HeatPoint * heat_points, float * heat_matrix, int * bool_matrix)
{
	float average = average_temperature(heat_points);

	for (int i = 0; i < RowDim*ColDim; ++i) 
		heat_matrix[i] = (bool_matrix[i] == 1) ? average : 0.0;

	for (int i = 0; i < HeatPointsDim; ++i) 
		heat_matrix[ heat_points[i].y * ColDim + heat_points[i].x ] = heat_points[i].heat;

}

void save_heat(float * matrix){
    FILE * pf = fopen("heat.dat","w");
    for (int i=0; i<RowDim; ++i){
        for (int j=0; j<ColDim; ++j) fprintf(pf, "%f\t", matrix[i * ColDim + j]);
        fprintf(pf, "\n");
    }
    fclose(pf);
}

void save_bool(int * matrix){
    FILE * pf = fopen("bool.dat","w");
    for (int i=0; i<RowDim; ++i){
        for (int j=0; j<ColDim; ++j) fprintf(pf, "%d\t", matrix[i * ColDim + j]);
        fprintf(pf, "\n");
    }
    fclose(pf);
}

int main(int argc, char const *argv[])
{
	int * B = (int *) malloc(RowDim*ColDim*sizeof(int));
	float * Tin = (float *) malloc(RowDim*ColDim*sizeof(float));
	float * Tout = (float *) malloc(RowDim*ColDim*sizeof(float));

	// initialize_bool(B);
	// save_bool(B);


	struct HeatPoint heat_points[HeatPointsDim];
	heat_points[0].x = 104;
	heat_points[0].y = 421;
	heat_points[0].heat = 30.00;

	heat_points[1].x = 281;
	heat_points[1].y = 125;
	heat_points[1].heat = 30.00;

	heat_points[2].x = 441;
	heat_points[2].y = 276;
	heat_points[2].heat = 30.00;

	heat_points[3].x = 473;
	heat_points[3].y = 585;
	heat_points[3].heat = 30.00;

	heat_points[4].x = 738;
	heat_points[4].y = 328;
	heat_points[4].heat = 30.00;

	heat_points[5].x =  793;
	heat_points[5].y = 540;
	heat_points[5].heat = 30.00;


	initialize_temperatures(heat_points,Tin,B);
	// initialize_temperatures(Tout);

	save_heat(Tin);
	free(B);
	free(Tin);
	free(Tout);
	return 0;
}