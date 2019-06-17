#ifndef BODY_H
#define BODY_H
#include "Quad.h"
#define G 6.673e-11		//gravitational constant
#define solarmass 1.98892e30 


class Body {
public:
	double rx, ry;       // holds the cartesian positions
	double vx, vy;       // velocity components 
	double fx, fy;       // force components
	double mass;         // mass

public:
	Body(double rx = 0.0, double ry = 0.0, double vx = 0.0, double vy = 0.0, double mass = 0.0);
	void update(double dt);
	double distanceTo(Body *b);
	void resetForce();
	void addForce(Body *b);
	bool in(Quad *q);
	static Body* merge(Body *a, Body *b);
};

#endif