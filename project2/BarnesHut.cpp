//#include<stdio.h>
//#include<stdlib.h>
//#include<math.h>
//#include"Body.h"
//#include<time.h>
//#include<omp.h>
//#include<mpi.h>
//#include"BHTree.h"
//
//#define PI 3.1415926
//#define rand (rand()%1000)/1000.0
//
//#define MASTER 0
//#define FROM_MASTER 1
//#define FROM_CHILD 2
//MPI_Status status;
////variables to state threads
//int myid = 0, processNum = 0;
//
//int N;
//Body *bodies;
//// radius of universe
//Quad *q = new Quad(0, 0, 2 * 1e18);
//
//double treeTime = 0;
//double calcTime = 0;
//
//void init() {
//	bodies = new Body[N];
//}
//double signum(double x) {
//	if (x == 0.0)
//		return 0.0;
//	else if (x > 0)
//		return 1.0;
//	else
//		return -1.0;
//}
//
////A function to return an exponential distribution for position
//double getExp(double lambda) {
//	return -log(1 - rand) / lambda;
//}
//
////the bodies are initialized in circular orbits around the central mass.
////This is just some physics to do that
//static double circlev(double rx, double ry) {
//
//	double r2 = sqrt(rx*rx + ry*ry);
//	double numerator = (6.67e-11)*1e6*solarmass;
//	return sqrt(numerator / r2);
//}
//
//
////Initialize N bodies with random positions and circular velocities
//void startthebodies(int N) {
//	double radius = 1e18;        // radius of universe
//	srand(time(0));
//
//	for (int i = 1; i < N; i++) {
//		double px = 1e18*getExp(-1.8)*(.5 - rand);
//		double py = 1e18*getExp(-1.8)*(.5 - rand);
//		double magv = circlev(px, py);
//
//		double absangle = atan(abs(py / px));
//		double thetav = PI / 2 - absangle;
//		double phiv = rand* PI;
//		double vx = -1 * signum(py)* cos(thetav)*magv;
//		double vy = signum(px)*sin(thetav)*magv;
//		//Orient a random 2D circular orbit
//
//		if (rand <= .5) {
//			vx = -vx;
//			vy = -vy;
//		}
//
//		double mass = rand*solarmass * 10 + 1e20;
//		
//		
//		bodies[i] = Body(px, py, vx, vy, mass);
//	}
//	//Put the central mass in
//	bodies[0] = Body(0, 0, 0, 0, 1e6*solarmass);//put a heavy body in the center
//
//}
//
////the duty of master is to build the tree and send the tree to all children processes
//void master() {
//
//}
////Use the method in Body to reset the forces, then add all the new forces
//void updateForces(int N) {
//	double treeStart = omp_get_wtime();
//	BHTree *thetree = new BHTree(q);
//	// If the body is still on the screen, add it to the tree
//	for (int i = 0; i < N; i++) {
//		if (bodies[i].in(q))
//		{
//			thetree->insert(&bodies[i]);
//		}
//	}
//	double treeEnd = omp_get_wtime();
//	treeTime += (treeEnd - treeStart);
//	//printf("tree building time: %f", treeEnd - treeStart);
//	//Now, use out methods in BHTree to update the forces,
//	//traveling recursively through the tree
//	double calcStart = omp_get_wtime();
//#pragma omp parallel for schedule(dynamic)
//	for (int i = 0; i < N; i++) {
//		bodies[i].resetForce();
//		if (bodies[i].in(q)) {
//			thetree->updateForce(&bodies[i]);
//			//Calculate the new positions on a time step dt (1e11 here)
//			bodies[i].update(1e5);
//		}
//	}
//	double calcEnd = omp_get_wtime();;
//	calcTime += (calcEnd - calcStart);
//	delete thetree;
//}
//
//int main(int argc, char *argv[]){
//	
//
//
//	sscanf(argv[1], "%d", &N);
//	init();
//	clock_t start = clock();
//	startthebodies(N);
//	
//	for (int i = 0; i < 100; i++) {
//		
//		//printf("body[1]: (%e,%e)  \n", bodies[0].rx, bodies[0].ry);
//		updateForces(N);
//		//printf("rand%f\n", 1e18*getExp(-1.8)*(.5 - rand));
//	}
//	//	
//	printf("avg tree time: %f\n", treeTime / 100);
//	printf("avg calculation time: %f\n", calcTime / 100);
//	clock_t end = clock();
//
//	printf("Barnes-Tree-OMP time used: %f", (double)(end - start) / CLOCKS_PER_SEC);
//
//	return 0;
//
//}