#include "BHTree.h"
#include<stdio.h>

//Create and initialize a new bhtree. Initially, all nodes are nullptr and will be filled by recursion
//Each BHTree represents a quadrant and a body that represents all bodies inside the quadrant
BHTree::BHTree(Quad *q) {
	quad = q;
	body = nullptr;
	NW = nullptr;
	NE = nullptr;
	SW = nullptr;
	SE = nullptr;
}
//If all nodes of the BHTree are null, then the quadrant represents a single body and it is "external"
bool BHTree::isExternal() {
	if (!NW  && !NE  && !SW  && !SE ) return true;
	else return false;
}
//We have to populate the tree with bodies. We start at the current tree and recursively travel through the branches
void BHTree::insert(Body *b) {
	//printf("in insert");
	//If there's not a body there already, put the body there.
	if (!body ) {
		body = b;
	}
	//If there's already a body there, but it's not an external node
	//combine the two bodies and figure out which quadrant of the 
	//tree it should be located in. Then recursively update the nodes below it.
	else if (!isExternal()) {
		body = b->merge(body, b);

		Quad *northwest = quad->NW();
		if (b->in(northwest)) {
			if (!NW ) { NW = new BHTree(northwest); }
			NW->insert(b);
		}
		else {
			Quad *northeast = quad->NE();
			if (b->in(northeast)) {
				if (!NE) { NE = new BHTree(northeast); }
				NE->insert(b);
			}
			else {
				Quad *southeast = quad->SE();
				if (b->in(southeast)) {
					if (!SE ) { SE = new BHTree(southeast); }
					SE->insert(b);
				}
				else {
					Quad *southwest = quad->SW();
					if (!SW ) { SW = new BHTree(southwest); }
					SW->insert(b);
				}
			}
		}
	}
	//If the node is external and contains another body, create BHTrees
	//where the bodies should go, update the node, and end 
	//(do not do anything recursively)
	else if (isExternal()) {
		Body *c = body;
		Quad *northwest = quad->NW();
		if (c->in(northwest)) {
			if (!NW ) { NW = new BHTree(northwest); }
			NW->insert(c);
		}
		else {
			Quad *northeast = quad->NE();
			if (c->in(northeast)) {
				if (!NE ) { NE = new BHTree(northeast); }
				NE->insert(c);
			}
			else {
				Quad *southeast = quad->SE();
				if (c->in(southeast)) {
					if (!SE ) { SE = new BHTree(southeast); }
					SE->insert(c);
				}
				else {
					Quad *southwest = quad->SW();
					if (!SW ) { SW = new BHTree(southwest); }
					SW->insert(c);
				}
			}
		}
		insert(b);
	}
}
//Start at the main node of the tree. Then, recursively go each branch
//Until either we reach an external node or we reach a node that is sufficiently
//far away that the external nodes would not matter much.
void BHTree::updateForce(Body *b) {
	if (isExternal()) {
		if (body != b) b->addForce(body);
	}
	else if (quad->length() / (body->distanceTo(b))<2){
		b->addForce(body);
	}
	else {
		if (NW != nullptr) NW->updateForce(b);
		if (SW != nullptr) SW->updateForce(b);
		if (SE != nullptr) SE->updateForce(b);
		if (NE != nullptr) NE->updateForce(b);
	}
}
