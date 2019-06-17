//#include<stdio.h>
//#include<stdlib.h>
//#include<math.h>
//#include"Body.h"
//#include<time.h>
//#include<omp.h>
//
//#define PI 3.1415926
//#define rand (rand()%1000)/1000.0
//
//int N = 5000;
//Body *bodies;
//
//void init() {
//	bodies = new Body[N];
//}
////the bodies are initialized in circular orbits around the central mass.
////This is just some physics to do that
//static double circlev(double rx, double ry) {
//	
//	double r2 =sqrt(rx*rx + ry*ry);
//	double numerator = (6.67e-11)*1e6*solarmass;
//	return sqrt(numerator / r2);
//}
//
//
//double signum(double x) {
//	if (x == 0.0)
//		return 0.0;
//	else if (x > 0)
//		return 1.0;
//	else
//		return -1.0;
//}
//
//double getExp(double lambda) {
//	return -log(1 - rand) / lambda;
//}
//
////Initialize N bodies with random positions and circular velocities
//void startthebodies(int N) {
//	double radius = 1e18;        // radius of universe
//	srand((unsigned)time(NULL));
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
////Use the method in Body to reset the forces, then add all the new forces
//void updateForces(int N) {
//#pragma omp parallel for
//	for (int i = 0; i < N; i++) {
//		bodies[i].resetForce();
//		//Notice-2 loops-->N^2 complexity
//		for (int j = 0; j < N; j++) {
//			if (i != j) bodies[i].addForce(&bodies[j]);
//		}
//	}
//	//Then, loop again and update the bodies using timestep dt
//	for (int i = 0; i < N; i++) {
//		bodies[i].update(1e5);
//	}
//}
//
//int main(int argc, char *argv[]){
//	sscanf(argv[1], "%d", &N);
//	init();
//
//	clock_t start = clock();
//
//	startthebodies(N);
//	for (int i = 0; i < 100; i++) {
//		
//		//printf("body[1]:  (%e, %e)\n", bodies[0].rx, bodies[0].ry);
//		updateForces(N);
//	}
//
//	clock_t end = clock();
//
//	printf("Brute-Force time used: %f", (double)(end - start) / CLOCKS_PER_SEC);
//	getchar();
//	return 0;
//}