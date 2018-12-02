#include "GuestTree.h"



GuestTree::GuestTree()
{
	Guest* rootGuest = new Guest();
	rootGuest->setGuestName("ROOT");
	this->root = new GuestNode(rootGuest,0,0);
}


GuestTree::~GuestTree()
{
	delete root;
}

vector<GuestNode*>* GuestTree::getPathExcludingRoot(GuestNode* node) {
	vector<GuestNode*>* path = new vector<GuestNode*>();
	GuestNode* current = node;
	while (current != nullptr && current != this->root) {
		path->push_back(current);
		current = current->getParent();
	}
	return path;
}

vector<GuestNode*>* GuestTree::getPathIncludingRoot(GuestNode* node) {
	vector<GuestNode*>* path = new vector<GuestNode*>();
	GuestNode* current = node;
	while (current != nullptr) {
		path->push_back(current);
		current = current->getParent();
	}
	return path;
}

GuestNode* GuestTree::getRoot() {
	return root;
}


