#ifndef QUAD_H
#define QUAD_H

class Quad {
public:
	double xmid, ymid, len;

public:
	//Create a square quadrant with a given length and midpoints (xmid,ymid)
	Quad(double xmid, double ymid, double length);
	double length();
	bool contains(double xmid, double ymid);
	Quad* NW();
	Quad* NE();
	Quad* SW();
	Quad* SE();
};



#endif