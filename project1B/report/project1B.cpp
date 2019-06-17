#include<cstdio>
#include <cstdlib>
#include <time.h>
#include<mpi.h>
#include<omp.h>
#include<cmath>

#define epsilon1 1E-10
#define epsilon2 -1E-10

#define MASTER 0
#define FROM_MASTER 1
#define FROM_CHILD 2
MPI_Status status;
//variables to state threads
int myid = 0, processNum = 0;


// return 1 if in set, 0 otherwise
int inset(double real, double img, int maxiter){
	double z_real = real;
	double z_img = img;
	double det = 0;
	for (int iters = 0; iters < maxiter; iters++){
		double z2_real = z_real*z_real - z_img*z_img;
		double z2_img = 2.0*z_real*z_img;
		//add comparation
		/*if ( z_real == z2_real + real && z_img == z2_img + img)
			return 1;*/
		z_real = z2_real + real;
		z_img = z2_img + img;
		if (iters == maxiter / 10){
		double error = (z_real*z_real + z_img*z_img - det) / det;
		if (error<epsilon1 && error >epsilon2){
		return 1;
		}
		}
		det = z_real*z_real + z_img*z_img;
		if (det > 4.0) return 0;


	}
	return 1;
}

// count the number of points in the set, within the region
//lower means the min, step means increase step, num_* means how many points will be calculate, 
int mandelbrotSetCount(double real_lower, double img_lower, double real_step, double img_step, int num_real, int num_img, int maxiter){
	int count = 0;
	#pragma omp parallel for schedule(dynamic)
	for (int real = 0; real<num_real; real++){
		for (int img = 0; img<num_img; img++)
	#pragma omp atomic
			count += inset(real_lower + real * real_step, img_lower + img * img_step, maxiter);
	}
	//printf("count in func: %d\n", count);
	return count;
}
//the child process will call this function to compute the points and send
void worker(double real_lower, double img_lower, double real_step, double img_step, int num_real, int num_img, int maxiter) {
	//printf("scope: %lf %lf %lf %lf %d %d %d \n", real_lower, img_lower, real_step, img_step, num_real, num_img, maxiter);
	int count = mandelbrotSetCount(real_lower, img_lower, real_step, img_step, num_real, num_img, maxiter);
	//printf("child count: %d\n", count);
	MPI_Send(&count, sizeof(int), MPI_INT, MASTER, FROM_CHILD, MPI_COMM_WORLD);
}
void master() {
	int i = 1;
	//to store the result of workers
	int *childRes = (int*)calloc(16, sizeof(int)), totalCount = 0;

	for (i = 1; i < processNum; i++) {
		//receieve data
		MPI_Recv(childRes + i-1, sizeof(int), MPI_INT, i, FROM_CHILD, MPI_COMM_WORLD, &status);
		totalCount += childRes[i-1];
	}

	printf("%d\n", totalCount);
}
int main(int argc, char *argv[])

{
	
	//params for computing
	double real_lower = 0.0;
	double real_upper = 0;
	double img_lower = 0;
	double img_upper = 0;
	int num = 0;
	int maxiter = 0;

	
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &processNum);
	double start = clock();
	
	int num_regions = (argc - 1) / 6;


	for (int region = 0; region<num_regions; region++){
		// scan the arguments
		sscanf(argv[region * 6 + 1], "%lf", &real_lower);
		sscanf(argv[region * 6 + 2], "%lf", &real_upper);
		sscanf(argv[region * 6 + 3], "%lf", &img_lower);
		sscanf(argv[region * 6 + 4], "%lf", &img_upper);
		sscanf(argv[region * 6 + 5], "%i", &num);
		sscanf(argv[region * 6 + 6], "%i", &maxiter);

		//calculate the step to increase
		double real_step = (real_upper - real_lower) / num;
		double img_step = (img_upper - img_lower) / num;
		//calculate the average number of points for each child
		int real_alloc_num = num / (processNum-1);
		//int img_alloc_num = num / processNum;

		//printf("parameters recv: %lf %lf %lf %lf %d %d \n", real_lower, real_upper, img_lower, img_upper, num, maxiter);

		if (myid == MASTER) {
			master();
		}
		else {// the last process will take all the left data
			//new real bound
			double new_real_lower = real_lower + (myid - 1) * real_alloc_num * real_step;
			int child_num_real = real_alloc_num;
			if (myid == processNum-1)
				child_num_real = real_alloc_num + num % (processNum-1);
			
			//new img bound
			/*double new_img_lower = img_lower + (myid - 1) * img_alloc_step;
			double new_img_upper = img_upper;*/
			worker(new_real_lower, img_lower, real_step, img_step, child_num_real, num, maxiter);
		}
		
	}
	/*sscanf(argv[1], "%lf", &real_lower);
	printf("Hello world from process %d of %d  %lf\n", myid, numprocs, real_lower);*/
	if (myid == MASTER)
		printf("time used: %9.7f\n", (clock() - start)/CLOCKS_PER_SEC);
	MPI_Finalize();

	
	return 0;

}

