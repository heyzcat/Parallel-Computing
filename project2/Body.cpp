#include<math.h>
#include "Body.h"



// create and initialize a new Body
Body::Body(double rx, double ry, double vx, double vy, double mass) {
	this->rx = rx;
	this->ry = ry;
	this->vx = vx;
	this->vy = vy;
	this->mass = mass;
}
// update the velocity and position using a timestep dt
void Body::update(double dt) {
	vx += dt * fx / mass;
	vy += dt * fy / mass;
	rx += dt * vx;
	ry += dt * vy;
}

// returns the distance between two bodies
double Body::distanceTo(Body *b) {
	double dx = rx - b->rx;
	double dy = ry - b->ry;
	return sqrt(dx*dx + dy*dy);
}

// set the force to 0 for the next iteration
void Body::resetForce() {
	fx = 0.0;
	fy = 0.0;
}

// compute the net force acting between the body a and b, and
// add to the net force acting on a
void Body::addForce(Body *b) {
	Body *a = this;
	double EPS = 3E4;      // softening parameter (just to avoid infinities)
	double dx = b->rx - a->rx;
	double dy = b->ry - a->ry;
	double dist = sqrt(dx*dx + dy*dy);
	double F = (G * a->mass * b->mass) / (dist*dist + EPS*EPS);
	a->fx += F * dx / dist;
	a->fy += F * dy / dist;
}

bool Body::in(Quad *q) {
	return q->contains(rx, ry);
}

Body* Body::merge(Body *a, Body *b) {
	Body *c = new Body();
	c->mass = a->mass + b->mass;
	c->rx = (a->rx*a->mass + b->rx*b->mass) / c->mass;
	c->ry = (a->ry*a->mass + b->ry*b->mass) / c->mass;
	return c;
}
