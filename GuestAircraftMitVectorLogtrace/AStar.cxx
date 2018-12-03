/* 18-November-2017
Copyright: H.Helmke,
Quellcode fuer den A*-Algorithmus
*/

#include "AStar.h"
#include "GuestArray.h"
// HHe 28.10.2018
//#include "PermutGuest.h"
#include "EvalSequence.h" // printSequenceTo
#include "TakeSelect.h"

#include "logtrace.h"


using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="EvalSequenceAStar";


AStar::AStar() : md_bestSeqValue(-1)
{
}

AStar::~AStar() {
	//delete tree;

}

/**
Die Funktion ermittelt eine gute Reihenfolge für die Gaeste
im Array guests für die Minimierung der Wartezeiten
bei der Badbelegung.
Hierzu wird der Take-Select unter Verwendung von vollständiger Permutation
als Basis verwendet.

\param[in]   guests  (Array)
\param[out]  bestSeq (Array)  // optimale Sequenz gemaess A*
\return      true, wenn erfolgreich
*/
bool AStar::getBestSequence
   (
   const GuestArray& guests,
   GuestArray& bestSeq
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequence", "");
   const int select = 3;
   const int take = 1;
   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);
   md_bestSeqValue = strategy.GetBestSeq(guests, bestSeq);

   tree = new GuestTree();

   vector<const Guest*> guestArr = vector<const Guest*>();
   for (int i = 0; i < guests.numberOfGuests(); i++) {
	   const Guest* temp = & guests.getGuest(i);
	   guestArr.push_back(temp);
   }

   GuestNode* min = generateTree(guestArr);

   while (min != nullptr) {
	   cout << min->toString() << endl;
	   min = min->getParent();
   }

   //cout << tree->getRoot()->subtreeToString() << endl;

   delete tree;
   return true;

}// getBestSequence

vector<GuestNode*>* AStar::expandAll(vector<GuestNode*>* toExpand, int maxExpand, vector<const Guest*>& closedList) {
	vector<GuestNode*>* expanded = new vector<GuestNode*>();
	for (int i = 0; i < toExpand->size() && i < maxExpand; i++) {
		toExpand->at(i)->expand(closedList);
		expanded->insert(expanded->end(), toExpand->at(i)->getChildren()->begin(), toExpand->at(i)->getChildren()->end());
	}

	toExpand->clear();
	delete toExpand;
	return expanded;
}

bool sortGuestNodes(GuestNode* gn1, GuestNode* gn2) {
	return (gn1->getFCost() < gn2->getFCost());
}

GuestNode* AStar::generateTree(vector<const Guest*>& entityList) {
	vector<GuestNode*>* toExpand = new vector<GuestNode*>();
	toExpand->push_back(tree->getRoot());
	while (!toExpand->empty()) {
		toExpand = expandAll(toExpand, 10, entityList);
		sort(toExpand->begin(), toExpand->end(), sortGuestNodes);
		if (toExpand->at(0)->isLeaf()) {
			return toExpand->at(0);
		}
	}
	cout << "Unexpected Result in generateTree" << endl;
	return nullptr;
}

static int compare(GuestNode& g1, GuestNode& g2) {
	if (g1.getFCost() < g2.getFCost())
		return -1;
	else if (g1.getFCost() > g2.getFCost())
		return 1;
	return 0;
}

GuestTree* AStar::getTree() {
	return tree;
}

int AStar::size() {
	int n = 1;
	tree->getRoot()->count(n);
	return n;
}