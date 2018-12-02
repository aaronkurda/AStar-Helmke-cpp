#include "GuestAStar.h"



GuestAStar::GuestAStar(vector<Guest*>* allGuests)
{
	this->allGuests = allGuests;
	openList = new vector<Guest*>();
	closedList = new vector<Guest*>();
	path = new vector<GuestNode*>();
	tree = new GuestTree();
}


GuestAStar::~GuestAStar()
{
	delete tree;
	delete openList;
	delete closedList;
	delete path;
	delete tree;
}

GuestNode* GuestAStar::lazyCutExpand(vector<Guest*>* closedList) {
	vector<GuestNode*>* currentDepth = new vector<GuestNode*>();
	currentDepth->push_back(tree->getRoot());
	while (!currentDepth->empty()) {
		vector<GuestNode*>* expanded = expandAll(currentDepth, closedList);
		delete currentDepth;
		currentDepth = expanded;
		//qsort(currentDepth,currentDepth->size(), sizeof(GuestNode), compareGuestNodes);
		
	}
	return nullptr;
}

vector<GuestNode*>* GuestAStar::expandAll(vector<GuestNode*>* toExpand, vector<Guest*>* guests) {
	vector<GuestNode*>* r = new vector<GuestNode*>;
	for (int i = 0; i < toExpand->size(); i++) {
		toExpand->at(i)->expand(guests);
		r->insert(r->end(), toExpand->at(i)->getChildren().begin(), toExpand->at(i)->getChildren().end());
	}
	return r;
}
