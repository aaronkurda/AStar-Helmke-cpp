#pragma once

#include <list>
#include <vector>
#include <Guest.h>
using namespace std;
class Node
{
public:
	Node(Guest* guest, Node* parent, int parentFinishTime, int parentFCost);
	Node(Guest* guest, int parentFinishTime, int parentFCost);
	~Node();
	//void removeAllChildren();
	//void removeParent();
	void isolate();

	void addChild(Node* child);
	void expand(vector<Guest*> guestDatas);

	bool isLeaf();
	vector<Guest*> getBloodline();

	int getFCost();
	vector<Node*> getChildren();
	Guest* getGuest();
	Node* getParent();
	int getCurrentTime();

	string toString();
	//subtreeToString
private:
	Guest* guest;
	vector<Node*> children;
	int fCost;
	int currentTime;
	Node* parent;
};

