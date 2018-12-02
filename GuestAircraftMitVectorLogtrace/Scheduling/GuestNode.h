#pragma once

#include <list>
#include <vector>
#include <Guest.h>
using namespace std;
class GuestNode
{
public:
	GuestNode(Guest* guest, GuestNode* parent, int parentFinishTime, int parentFCost);
	GuestNode(Guest* guest, int parentFinishTime, int parentFCost);
	~GuestNode();
	//void removeAllChildren();
	//void removeParent();
	void isolate();

	void addChild(GuestNode* child);
	void expand(vector<Guest*>* guestDatas);

	bool isLeaf();
	vector<Guest*> getBloodline();

	void setFCost(int fCost);

	int getFCost();
	vector<GuestNode*> getChildren();
	Guest* getGuest();
	GuestNode* getParent();
	int getCurrentTime();

	bool operator>(GuestNode& op2);
	bool operator<(GuestNode& op2);
	bool operator==(GuestNode& op2);

	string toString();
	string subtreeToString();
private:
	Guest* guest;
	vector<GuestNode*> children;
	int fCost;
	int currentTime;
	GuestNode* parent;
};

