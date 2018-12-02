#include "Node.h"
#include <vector>
using namespace std;



Node::Node(Guest* guest, Node* parent, int parentFinishTime, int parentFCost)
{
	this->guest = guest;
	this->parent = parent;
	this->children = vector<Node*>();
	if (parentFinishTime > guest->getWakeUpTime()) {
		this->currentTime = parentFinishTime;
		this->fCost = parentFCost + (parentFinishTime - guest->getWakeUpTime());
	}
	else {
		this->currentTime = guest->getWakeUpTime();
		this->fCost = parentFCost;
	}
}

Node::Node(Guest* guest, int parentFinishTime, int parentFCost)
{
	this->guest = guest;
	this->children = vector<Node*>();
	if (parentFinishTime > guest->getWakeUpTime()) {
		this->currentTime = parentFinishTime;
		this->fCost = parentFCost + (parentFinishTime - guest->getWakeUpTime());
	}
	else {
		this->currentTime = guest->getWakeUpTime();
		this->fCost = parentFCost;
	}
}



/*
Der der Destruktor von Node löscht rekursiv erst alle Kinder der Node und danach
sich selbst. Sollten die KInder selbst auch Kinder haben, werden auch diese gelöscht.
Um also nur eine Node zu löschen und seine Kinder nicht, ist es wichtig die Kinder
vorher aus dem Vector zu entfernen.
*/
Node::~Node()
{
	this->children.clear();
	delete &children;
}

/*
Löscht die referenz auf dieses Objekt aus parent.children
und löscht zudem die Referenz von parent aus diesem Objekt.
Zudem werden alle Kinder dieses Objektes zerstört.
TODO: NICHT FERTIG
*/
void Node::isolate() {
	//this->parent->children->erase(this)
	this->parent = nullptr;
	delete &children;
}

void Node::addChild(Node* child)
{
	this->children.push_back(child);
}

void Node::expand(vector<Guest*> guestDatas) {
	vector<Guest*> bloodline = getBloodline();
	bool childrenContainGuest;
	bool bloodlineContainsGuest;
	for (int g = 0; g < guestDatas.size(); g++) {
		childrenContainGuest = false;
		bloodlineContainsGuest = false;
		for (int c = 0; c < this->children.size(); c++) {
			if (this->children.at(c)->guest == guestDatas.at(g)) {
				childrenContainGuest = true;
				break;
			}
		}
		if (childrenContainGuest == false) {
			for (int b = 0; b < bloodline.size(); b++) {
				if (bloodline.at(b) == guestDatas.at(g)) {
					bloodlineContainsGuest = true;
				}
			}
			if (bloodlineContainsGuest == false) {
				//guestData ist weder in der bloodline, als auch in den den Kindern zu finden.
				//Ein neues Kind mit guestData kann angelegt werden
				Node* n = new Node(guestDatas.at(g), this, this->currentTime + this->guest->getBathUsageTime(), this->fCost);
				this->addChild(n);
			}
		}
	}
}

vector<Guest*> Node::getBloodline()
{
	vector<Guest*> bloodline;
	Node* current = this;
	while (current != nullptr) {
		bloodline.push_back(current->getGuest());
		current = current->getParent();
	}
	return bloodline;
}

vector<Node*> Node::getChildren() {
	return this->children;
}

Guest* Node::getGuest() {
	return this->guest;
}

Node* Node::getParent() {
	return this->parent;
}

int Node::getCurrentTime() {
	return this->currentTime;
}

string Node::toString() {
	return guest->getGuestName();
}