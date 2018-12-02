#pragma once
#include "Scheduling/GuestNode.h"

class GuestTree
{
public:
	GuestTree();
	~GuestTree();
	vector<GuestNode*>* getPathExcludingRoot(GuestNode* node);
	vector<GuestNode*>* getPathIncludingRoot(GuestNode* node);

	GuestNode* getRoot();
private:
	GuestNode* root;
};

