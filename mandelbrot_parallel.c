#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <complex.h>
#include <math.h>
/*Code by Marie Kindblom & Michel Postigo Smura. */
const int N = 256;
const int b = 2;
const int w = 4048;
const int h = 4048;
static int num_elem_per_process = 0;

static int cal_pixel(double complex d, int b, int N){
    int count = 1;
    double complex z = 0;
    double complex z_prev = 0;
    while ((cabs(z) < b) && (count < N)){
        z_prev = z;
        z = z*z + d;
        count = count+1;
    }

    if (cabs(z) < b && count >= N){
        return 256;   
    } 
    else {    //if ((cabs(z) >= b) && cabs(z_prev) < b){
        return count;
    }
}

int *mandelbrot(int start, int end){
	/*For normal settings, use magnify = 1, 
      offset_x = 0, offset_y = 0*/
    int magnify = 1; 
    int offset_x = 0; 
    int offset_y = 0;

	double dx = (double)(2*b)/(double)(magnify*w-1);
	double dy = (double)(2*b)/(double)(magnify*h-1);
	double d_real;
	double d_imag;
	int *vec = (int *) malloc(num_elem_per_process*sizeof(int));
	
	for (int lx = 0; lx < num_elem_per_process/h; lx++){
		int x = start/h + lx + offset_x; 
		d_real = (double)(x*dx)-(double)(b);

		for (int y = 0; y < h; y++){

			d_imag = (double)((y+offset_y)*dy)-(double)(b);
			double complex d = d_real+I*d_imag;
			int color = cal_pixel(d, b, N);
			vec[lx*h + y] = color;
		}
	}
	return vec;

}

int main(int argc, char **argv){

	int rank, size, tag, rc, i;
	MPI_Status status;
	char message[20];
	int res;
	rc = MPI_Init(&argc, &argv);
	rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	num_elem_per_process = (h*w)/(size-1);

	tag = 100;

	if (rank == 0) {

		FILE *fp = fopen("/home/m/a/marieki/kurser/parpro/labb1/test.txt", "w+");

		if (fp == NULL){
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		int* matrix= (int *)malloc(h*w*sizeof(int));
		int* array = (int *)malloc(num_elem_per_process*sizeof(int));
		array=mandelbrot(0, num_elem_per_process);
		for (i = 1; i < size; i++) {
			assert(array != NULL);
			// Not optimal if last process finishes first.
			rc = MPI_Recv(array, num_elem_per_process, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
			for (int j = 0; j < num_elem_per_process; j++){
				matrix[j+num_elem_per_process*(i-1)] = array[j];
			}
			assert(rc == MPI_SUCCESS);
		}
		for (int a = 0; a < w; a++){
			for (int b = 0; b < h; b++){
				fprintf(fp, "%d ", matrix[a+b*h]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	} else {
		int start = (rank-1)*num_elem_per_process; //CHECK THAT DIVISION IS INT
		int end = start + num_elem_per_process;
		int *array = mandelbrot(start, end);
		rc = MPI_Send(array, num_elem_per_process, MPI_INT, 0, tag, MPI_COMM_WORLD);
		assert(rc == MPI_SUCCESS);
	}


	rc = MPI_Finalize();
	return 0;
}
