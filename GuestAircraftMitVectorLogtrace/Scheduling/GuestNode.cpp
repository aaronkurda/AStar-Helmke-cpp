#include "GuestNode.h"
#include <vector>
#include <iostream>
using namespace std;



GuestNode::GuestNode(const Guest* guest, GuestNode* parent, int parentFinishTime, int parentFCost)
{
	this->guest = guest;
	this->parent = parent;
	this->children = new vector<GuestNode*>();
	if (parentFinishTime > guest->getWakeUpTime()) {
		this->currentTime = parentFinishTime;
		this->fCost = parentFCost + (parentFinishTime - guest->getWakeUpTime());
	}
	else {
		this->currentTime = guest->getWakeUpTime();
		this->fCost = parentFCost;
	}
}

GuestNode::GuestNode(const Guest* guest, int parentFinishTime, int parentFCost)
{
	this->guest = guest;
	this->parent = nullptr;
	this->children = new vector<GuestNode*>();
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
GuestNode::~GuestNode()
{
	if (children != NULL) {
		if(children->size() > 0)
			this->children->clear();
		delete children;
	}
}

/*
Löscht die referenz auf dieses Objekt aus parent.children
und löscht zudem die Referenz von parent aus diesem Objekt.
Zudem werden alle Kinder dieses Objektes zerstört.
TODO: NICHT FERTIG
*/
void GuestNode::isolate() {
	//this->parent->children->erase(this)
	this->parent = nullptr;
	delete &children;
}

void GuestNode::addChild(GuestNode* child)
{
	this->children->push_back(child);
}

void GuestNode::expand(vector<const Guest*>& guestDatas) {
	vector<const Guest*> bloodline = getBloodline();
	bool childrenContainGuest;
	bool bloodlineContainsGuest;
	for (int g = 0; g < guestDatas.size(); g++) {
		childrenContainGuest = false;
		bloodlineContainsGuest = false;
		for (int c = 0; c < this->children->size(); c++) {
			if (this->children->at(c)->guest == guestDatas.at(g)) {
				childrenContainGuest = true;
				break;
			}
		}
		if (childrenContainGuest == false) {
			for (int b = 0; b < bloodline.size(); b++) {
				if (bloodline.at(b) == guestDatas.at(g)) {
					bloodlineContainsGuest = true;
					break;
				}
			}
			if (bloodlineContainsGuest == false) {
				//guestData ist weder in der bloodline, als auch in den den Kindern zu finden.
				//Ein neues Kind mit guestData kann angelegt werden
				GuestNode* n = new GuestNode(guestDatas.at(g), this, this->currentTime + this->guest->getBathUsageTime(), this->fCost);
				this->addChild(n);
			}
		}
	}
}

bool GuestNode::isLeaf() {
	if (!this->parent || this->parent->getChildren()->size() <= 1) {
		return true;
	}
	return false;
}

vector<const Guest*> GuestNode::getBloodline()
{
	vector<const Guest*> bloodline;
	GuestNode* current = this;
	while (current != nullptr) {
		bloodline.push_back(current->getGuest());
		current = current->getParent();
	}
	return bloodline;
}

void GuestNode::setFCost(int fCost) {
	this->fCost = fCost;
}

vector<GuestNode*>* GuestNode::getChildren() {
	return this->children;
}

const Guest* GuestNode::getGuest() {
	return this->guest;
}

GuestNode* GuestNode::getParent() {
	return this->parent;
}

int GuestNode::getCurrentTime() {
	return this->currentTime;
}

int GuestNode::getFCost() {
	return this->fCost;
}

string GuestNode::toString() {
	return "[" + guest->getGuestName() + "," + to_string(guest->getWakeUpTime()) + "," + to_string(guest->getBathUsageTime()) + "|" + to_string(this->currentTime) + "," + to_string(this->fCost) + "]";
}

string GuestNode::subtreeToString() {
	string s = "[" + guest->getGuestName() + "," + to_string(guest->getWakeUpTime()) + "," + to_string(guest->getBathUsageTime()) + "|" + to_string(this->currentTime) + "," + to_string(this->fCost) + "-->{";
	for (int i = 0; i < children->size(); i++) {
		s.append(children->at(i)->subtreeToString());
	}
	s.append("}]");
	return s;
}

bool GuestNode::operator>(GuestNode& op2) {
	if (this->getFCost() > op2.getFCost()) {
		return true;
	}
	return false;
}
bool GuestNode::operator<(GuestNode& op2) {
	if (this->getFCost() < op2.getFCost()) {
		return true;
	}
	return false;
}
bool GuestNode::operator==(GuestNode& op2) {
	if (this->getFCost() == op2.getFCost()) {
		return true;
	}
	return false;
}

bool GuestNode::operator>(GuestNode* op2) {
	if (this->getFCost() > op2->getFCost()) {
		return true;
	}
	return false;
}
bool GuestNode::operator<(GuestNode* op2) {
	if (this->getFCost() < op2->getFCost()) {
		return true;
	}
	return false;
}
bool GuestNode::operator==(GuestNode* op2) {
	if (this->getFCost() == op2->getFCost()) {
		return true;
	}
	return false;
}

void GuestNode::count(int& n) {
	n += children->size();
	for (GuestNode* gn : (*children)) {
		gn->count(n);
	}
}

void GuestNode::setParent(GuestNode* parent) {
	this->parent = parent;
}

void GuestNode::removeFromParent() {
	if (this->parent != nullptr) {
		vector<GuestNode*>* children = this->getParent()->getChildren();
		for (int i = 0; i < children->size(); i++) {
			if (children->at(i) == this) {
				children->erase(children->begin() + i);
				return;
			}
		}
	}
}

void GuestNode::deleteChildren() {
	if (this->children != nullptr) {
		this->children->clear();
		delete this->children;
	}
}