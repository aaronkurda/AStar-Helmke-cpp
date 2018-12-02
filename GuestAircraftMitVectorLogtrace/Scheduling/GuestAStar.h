
#pragma once
#include <Guest.h>
#include <vector>
#include "Scheduling/GuestNode.h"
#include "Scheduling/GuestTree.h"

class GuestAStar
{
public:
	GuestAStar(vector<Guest*>* allGuests);
	~GuestAStar();
	GuestNode* lazyCutExpand(vector<Guest*>* closedList);
	vector<GuestNode*>* expandAll(vector<GuestNode*>* toExpand, vector<Guest*>* guests);

private:
	vector<Guest*>* allGuests;
	vector<Guest*>* openList;
	vector<Guest*>* closedList;
	vector<GuestNode*>* path;
	GuestTree* tree;
};

