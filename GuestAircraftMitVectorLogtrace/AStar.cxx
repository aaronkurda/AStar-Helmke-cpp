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

   return true;

}// getBestSequence

