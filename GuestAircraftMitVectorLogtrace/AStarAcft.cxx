/* 18-November-2017
Copyright: H.Helmke,
Quellcode fuer den A*-Algorithmus
*/

#include "AStarAcft.h"
#include "TakeSelect.h"
#include "QualityFuncAircraft.h"

#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="AStarAcft";


AStarAcft::AStarAcft(const QualityFuncAircraft& a_qualiFunc) 
: md_bestSeqValue(-1),
  m_qualiFunc(a_qualiFunc)
{

}


/**
Die Funktion ermittelt die optimale Reihenfolge für die Flugzeuge
im Array inpAcftVect für die Minimierung der Qualitätsfunktion
Die beste Bewertung wird im Attribut mdbestSeqValue abgespeichert.
Hierzu wird der A*-Algorithmus verwendet.

\param[in]   inpAcftVect  (Array)
\param[out]  bestAcftSeq  // optimale Sequenz gemaess A*
\return      true, wenn erfolgreich
*/
bool AStarAcft::getBestSequence
   (
   const vector<Aircraft>& inpAcftVect, 
   vector<Aircraft>& bestAcftSeq
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequence", "");

   const int select = 5;
   const int take = 1;
   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);
   md_bestSeqValue = strategy.GetBestQualiClass(inpAcftVect, bestAcftSeq, m_qualiFunc);
   return true;

}// AStarAcft::getBestSequence

