#pragma once

#include <list>
#include <vector>
#include <Guest.h>
using namespace std;
class GuestNode
{
public:
	GuestNode(const Guest* guest, GuestNode* parent, int parentFinishTime, int parentFCost);
	GuestNode(const Guest* guest, int parentFinishTime, int parentFCost);
	~GuestNode();
	//void removeAllChildren();
	//void removeParent();
	void isolate();

	void addChild(GuestNode* child);
	void expand(vector<const Guest*>& guestDatas);

	bool isLeaf();
	vector<const Guest*> getBloodline();

	void setFCost(int fCost);

	int getFCost();
	vector<GuestNode*>* getChildren();
	const Guest* getGuest();
	GuestNode* getParent();
	void setParent(GuestNode* parent);
	void removeFromParent();
	int getCurrentTime();

	bool operator>(GuestNode& op2);
	bool operator<(GuestNode& op2);
	bool operator==(GuestNode& op2);

	bool operator>(GuestNode* op2);
	bool operator<(GuestNode* op2);
	bool operator==(GuestNode* op2);

	void count(int& n);

	string toString();
	string subtreeToString();
private:
	const Guest* guest;
	vector<GuestNode*>* children;
	int fCost;
	int currentTime;
	GuestNode* parent;
};

