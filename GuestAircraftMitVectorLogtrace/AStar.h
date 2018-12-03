/* 18-November-2017
Copyright: H.Helmke,
Header-Datei fuer den A*-Algorithmus
*/

#ifndef AStar_HEADER
#define AStar_HEADER

#include <list>
#include <vector>>
#include "Scheduling/GuestNode.h"
#include "Scheduling/GuestTree.h"

class AStar
{
public:
   AStar();
   ~AStar();
   bool getBestSequence(
      const class GuestArray& guests, class GuestArray& bestSeq);
   double getEvalForBestSeq() const  { return md_bestSeqValue;}

   vector<GuestNode*>* expandAll(vector<GuestNode*>* toExpand,int maxExpand, vector<const Guest*>& closedList);
   GuestNode* generateTree(vector<const Guest*>& entityList);

   GuestTree* getTree();
   int size();
private:
   double md_bestSeqValue;

   GuestTree* tree;

};


#endif /* AStar_HEADER */
