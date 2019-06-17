#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Body.h"
#include<time.h>
#include<omp.h>
#include<mpi.h>
#include"BHTree.h"

#define PI 3.1415926
#define rand (rand()%1000)/1000.0

#define MASTER 0
#define FROM_MASTER 1
#define FROM_CHILD 2
MPI_Status status;
//variables to state threads
int myid = 0, processNum = 0;
//new data type 
MPI_Datatype myType;

int N;
Body *bodies;
BHTree *root;
int bc[8];
// radius of universe
Quad *q = new Quad(0, 0, 2 * 1e18);

double treeTime = 0;
double calcTime = 0;

double signum(double x) {
	if (x == 0.0)
		return 0.0;
	else if (x > 0)
		return 1.0;
	else
		return -1.0;
}

//A function to return an exponential distribution for position
double getExp(double lambda) {
	return -log(1 - rand) / lambda;
}

//the bodies are initialized in circular orbits around the central mass.
//This is just some physics to do that
static double circlev(double rx, double ry) {

	double r2 = sqrt(rx*rx + ry*ry);
	double numerator = (6.67e-11)*1e6*solarmass;
	return sqrt(numerator / r2);
}


//Initialize N bodies with random positions and circular velocities
void initBodies() {
	

	double radius = 1e18;        // radius of universe
	srand(time(0));

	for (int i = 0; i < N; i++) {
		double px = 1e18*getExp(-1.8)*(.5 - rand);
		double py = 1e18*getExp(-1.8)*(.5 - rand);
		double magv = circlev(px, py);

		double absangle = atan(abs(py / px));
		double thetav = PI / 2 - absangle;
		double phiv = rand* PI;
		double vx = -1 * signum(py)* cos(thetav)*magv;
		double vy = signum(px)*sin(thetav)*magv;
		//Orient a random 2D circular orbit

		if (rand <= .5) {
			vx = -vx;
			vy = -vy;
		}

		double mass = rand*solarmass * 10 + 1e20;


		bodies[i] = Body(px, py, vx, vy, mass);
	}
	//Put the central mass in
	bodies[0] = Body(0, 0, 0, 0, 1e6*solarmass);//put a heavy body in the center

}



//Use the method in Body to reset the forces, then add all the new forces
void updateForces() {
	int step = N / processNum;
	int start = (myid ) * step;
	int end = (myid+1) * step;
	if (myid == processNum - 1)
		end = N;
	double treeStart = MPI_Wtime();
	BHTree *thetree = new BHTree(q);
	// If the body is still on the screen, add it to the tree
	for (int i = 0; i < N; i++) {
		if (bodies[i].in(q)) thetree->insert(&bodies[i]);
	}
	double treeEnd = MPI_Wtime();
	treeTime += (treeEnd - treeStart);
	//printf("tree building time: %f", treeEnd - treeStart);
	//printf("N is %d\n", N);
	//Now, use out methods in BHTree to update the forces,
	//traveling recursively through the tree
	double calcStart = omp_get_wtime();
//#pragma omp parallel for schedule(dynamic)
	for (int i = start; i < end; i++) {
		bodies[i].resetForce();
		if (bodies[i].in(q)) {
			thetree->updateForce(&bodies[i]);
			//Calculate the new positions on a time step dt (1e11 here)
			bodies[i].update(1e5);
		}
	}
	double calcEnd = omp_get_wtime();
	calcTime += (calcEnd - calcStart);
	delete thetree;

}

int main(int argc, char *argv[]){
	//clock_t start = clock();
	
	MPI_Init(&argc, &argv);
	
	//prepare for a new type
	double start = MPI_Wtime();
	int blockLength[] = { 7 };
	MPI_Datatype oldTypes[] = { MPI_DOUBLE };
	MPI_Aint addressOffsets[] = { 0 };
	//create a new type
	MPI_Type_create_struct(sizeof(blockLength) / sizeof(int),
		blockLength,
		addressOffsets,
		oldTypes,
		&myType);
	MPI_Type_commit(&myType);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &processNum);

	sscanf(argv[1], "%d", &N);
	bodies = new Body[N];
	if (myid == MASTER)
		initBodies(); 

	//master process to init all bodies -- must
	//coz all the processes generate the same rand value even srand is used to seed
	MPI_Bcast(bodies, N, myType, MASTER, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	for (int i = 0; i < 100; i++) {
		//update force here
		updateForces();
		//after each iteration of update, broadcast their new body data to other processes
		for (int i = 0; i < processNum - 1; i++) {
			int step = N / processNum;
			int start = (i)* step;
			int end = (i + 1)* step;
			if (i == processNum - 1)
				end = N;

			MPI_Bcast(&bodies[i], end - start , myType, i, MPI_COMM_WORLD);
			
		}
		MPI_Barrier(MPI_COMM_WORLD);
		
	}
	//printf("avg tree time: %f\n", treeTime / 100);
	//printf("avg calculation time: %f\n", calcTime / 100);
	//MPI_Barrier(MPI_COMM_WORLD);
	//clock_t end = clock();
	double end = MPI_Wtime();
	if (myid == MASTER)
		printf("Barnes-Tree-MPI time used: %f", (double)(end - start));

	MPI_Finalize();

	return 0;

}