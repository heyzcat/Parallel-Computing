#include "Quad.h"

//Create a square quadrant with a given length and midpoints (xmid,ymid)
Quad::Quad(double xmid, double ymid, double length) {
	this->xmid = xmid;
	this->ymid = ymid;
	this->len = length;
}
//How long is this quadrant?
double Quad::length() {
	return this->len;
}

//Check if the current quadrant contains a point
bool Quad::contains(double xmid, double ymid) {
	if (xmid <= this->xmid + this->len / 2.0 && xmid >= this->xmid - this->len / 2.0 && ymid <= this->ymid + this->len / 2.0 && ymid >= this->ymid - this->len / 2.0) {
		return true;
	}
	else {
		return false;
	}
}
//Create subdivisions of the current quadrant

// Subdivision: Northwest quadrant
Quad* Quad::NW() {
	Quad *newquad = new Quad(xmid - len / 4.0, ymid + len / 4.0, len / 2.0);
	return newquad;
}

// Subdivision: Northeast quadrant
Quad* Quad::NE() {
	Quad* newquad = new Quad(xmid + len / 4.0, ymid + len / 4.0, len / 2.0);
	return newquad;
}

// Subdivision: Southwest quadrant
Quad* Quad::SW() {
	Quad* newquad = new Quad(xmid - len / 4.0, ymid - len / 4.0, len / 2.0);
	return newquad;
}

// Subdivision: Southeast quadrant
Quad* Quad::SE() {
	Quad* newquad = new Quad(xmid + len / 4.0, ymid - len / 4.0, len / 2.0);
	return newquad;
}


