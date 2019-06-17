#ifndef BHTREE_H
#define BHTREE_H
#include "Body.h"
#include "Quad.h"

class BHTree {
private:
	Body *body;     // body or aggregate body stored in this node
	Quad *quad;     // square region that the tree represents
	BHTree *NW;     // tree representing northwest quadrant
	BHTree *NE;     // tree representing northeast quadrant
	BHTree *SW;     // tree representing southwest quadrant
	BHTree *SE;     // tree representing southeast quadrant

public:
	BHTree(Quad *q);
	bool isExternal();
	void insert(Body *b);
	void updateForce(Body *b);

};
#endif