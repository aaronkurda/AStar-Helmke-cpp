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
	cout << "---------------CALLED BEST SEQUENCE--------------" << endl;
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequence", "");
   const int select = 11;
   const int take = 2;
   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);
   md_bestSeqValue = strategy.GetBestSeq(guests, bestSeq);

   tree = new GuestTree();

   vector<const Guest*> guestArr = vector<const Guest*>();
   for (int i = 0; i < guests.numberOfGuests(); i++) {
	   const Guest* temp = & guests.getGuest(i);
	   guestArr.push_back(temp);
   }
   vector<const Guest*>* path = tsExpand(&guestArr, 11, 2);
   //cout << "Best Sequence cost: " << path->at(path->size() - 1)->getFCost() << endl;
   for (int i = 0; i < path->size(); i++) {
	   bestSeq.setGuest(i, *path->at(i));
	   cout << i << path->at(i)->getGuestName() << endl;
   }
   delete tree;
   delete path;
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

bool sortGuests(const Guest* g1, const Guest* g2) {
	return (g1->getWakeUpTime() < g2->getWakeUpTime());
}

vector<const Guest*>* AStar::tsExpand(vector<const Guest*>* allEntities, int take, int select) {
	cout << "Starting A* with " << take << "/" << select << endl;
	vector<const Guest*> openList = vector<const Guest*>();
	for (int i = 0; i < allEntities->size(); i++) {
		openList.push_back(allEntities->at(i));
	}
	sort(openList.begin(), openList.end(), sortGuests);
	vector<const Guest*> closedList = vector<const Guest*>();
	
	for (int i = 0; i < openList.size() && i < take; i++) {
		closedList.push_back(openList.at(i));
	}

	for (int i = 0; i < closedList.size(); i++) {
		openList.erase(openList.begin());
	}

	//cout << "Initialised openList with " << openList.size() << " Elements." << endl;
	//cout << "Initialised closedList with " << closedList.size() << " Elements." << endl;

	vector<GuestNode*>* path = new vector<GuestNode*>();

	vector<const Guest*>* guestPath = new vector<const Guest*>();
	vector<int>* costPath = new vector<int>();

	int removeChildrenCounter = 0;

	while (!closedList.empty()) {
		GuestNode* minimum = lazyCutExpand(closedList);
		vector<GuestNode*>* pathFraction = tree->getPathExcludingRoot(minimum);

		//cout << size() << endl;
		//cout << tree->getRoot()->subtreeToString() << endl;


		/*cout << "OPENLIST:" << endl;
		for (int i = 0; i < openList.size(); i++) {
			cout << openList.at(i)->getGuestName() << endl;
		}
		cout << "CLOSEDLIST:" << endl;
		for (int i = 0; i < closedList.size(); i++) {
			cout << closedList.at(i)->getGuestName() << endl;
		}*/

		for (int i = 0; i < select && i < pathFraction->size(); i++) {
			GuestNode* temp = pathFraction->at(i);
			if (temp != pathFraction->at(select < pathFraction->size() ? select : pathFraction->size() - 1)) {
				temp->removeFromParent();
			}
			
			path->push_back(temp);
			guestPath->push_back(temp->getGuest());
			costPath->push_back(temp->getFCost());
			

			for (int r = 0; r < closedList.size(); r++) {
				if (closedList.at(r) == temp->getGuest()) {
					closedList.erase(closedList.begin()+r);
				}
			}
			
			if (!openList.empty()) {
				closedList.push_back(openList.at(0));
				openList.erase(openList.begin());
			}
		}
		GuestNode* temp = path->at(path->size() - 1);
		temp->removeFromParent();
		//for (; removeChildrenCounter < path->size() - 1; removeChildrenCounter++) {
		//	path->at(removeChildrenCounter)->deleteChildren();
		//}

		delete tree->getRoot();
		tree->setRoot(path->at(path->size() - 1));
	}

	cout << costPath->at(costPath->size() - 1) << endl;

	/*for (int i = 0; i < path->size(); i++) {
		cout << path->at(i) << endl;
	}*/
	return guestPath;
	

}

GuestNode* AStar::lazyCutExpand(vector<const Guest*>& closedList) {
	vector<GuestNode*>* toExpand = new vector<GuestNode*>();
	toExpand->push_back(tree->getRoot());
	while (!toExpand->empty()) {
		toExpand = expandAll(toExpand, 100, closedList);
		sort(toExpand->begin(), toExpand->end(), sortGuestNodes);
		if (toExpand->at(0)->isLeaf()) {
			//cout << "Best Sequence cost: " << toExpand->at(0)->getFCost() << endl;
			return toExpand->at(0);
		}
	}
	cout << "Unexpected Result in generateTree" << endl;
	return nullptr;
}

GuestTree* AStar::getTree() {
	return tree;
}

int AStar::size() {
	int n = 1;
	tree->getRoot()->count(n);
	return n;
}