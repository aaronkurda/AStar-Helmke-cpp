/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Funktionalität der Sequenzplanung und -bewertung
*/
#include "EvalSeqAircraft.h"
#include "Aircraft.h"
#include "QualityFuncAircraft.h"
#include <vector>
#include <algorithm> // function next_permutation
#include "logtrace.h"
#include <cfloat>  // DBL_MAX
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="EvaluateSeqAcft";



double evaluateTimeSquareAcft
   (
   int refTime,
   int targetTime,
   double
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? diff*diff : 0;
}


static Zeit calcSep
   (
   const Aircraft& follower,
   const Aircraft& first
   )
{
/*   Heavy, Medium, Light, A380} */
static const Zeit sepMatrix[WeightClassNumber][WeightClassNumber] = {{100, 125, 150, 100},
                  {75, 75, 125, 75},
                  {75,75,75,75},
                  {125, 150, 200, 100}};
   return sepMatrix[first.getWeightClass()][follower.getWeightClass()];
}


/**
Bewertung einer Sequenz in seq aus Flugzeugen.

\param       seq
\param[in]   vorgElem
\param[in]   func, evaluation function
\param[in]   expo, parameter maybe used by func
\return Sequenzbewertung
*/
double evaluateSequenceFunc
   (
   vector<Aircraft>& seq,
   const Aircraft&  vorgElem,
   T_EvalFunc func,
   double expo
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "evaluateSequenceFunc", "");

   Zeit targTime = max(seq[0].getEarliestTime(),
      vorgElem.getTargetTime() + calcSep(seq[0], vorgElem));

   seq[0].setTargetTime(targTime);
   double seqValue = func(seq[0].getEarliestTime(), targTime, expo);
   seqValue += func(seq[0].getLatestTime(), targTime, expo);
   seqValue += func(seq[0].getlastSchedule(), targTime, expo);

   for (int i=1; i < static_cast<int>(seq.size()); ++i)
   {
      // Maximum aus Aufstehzeit und Verlassen durch Vorgänger
      targTime = max(seq[i].getEarliestTime(),
         seq[i-1].getTargetTime() + calcSep(seq[i], seq[i-1]));
      seq[i].setTargetTime(targTime);
      seqValue += func(seq[i].getEarliestTime(), targTime, expo);
      seqValue += func(seq[i].getLatestTime(), targTime, expo);
      seqValue += func(seq[i].getlastSchedule(), targTime, expo);
   }// for i

   TRACE("Evaluation of seq");
   for (int i=0; i < static_cast<int>(seq.size()); ++i) {
      TRACE("  " << seq[i]);
   }
   TRACE("   Quality is " << seqValue);

   return seqValue;

}// evaluateSequenceFunc



/**  Die Funktion erstellt eine Permuation von orgArr,
     wie sie im Array indexArr beschrieben ist, d.h.
     das Element i aus dem Array orgArr wird an die Stelle
     indexArr[i] im Array permutSeq kopiert.
\param[in]    orgArr
\param[out]   permutSeq,
\param[in]    indexArr
*/
void makeAircraftPermutation
   (
   const vector<Aircraft>& orgArr,
   vector<Aircraft>& permutSeq,
   const int indexArr[]
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "makeAircraftPermutation", "");
   for (int i=0; i < static_cast<int>(orgArr.size()); ++i)
   {      
      permutSeq[indexArr[i] ] = orgArr[i];
   }
} // makeAircraftPermutation


/**  Die Funktion erstellt eine Permuation von orgArr,
     wie sie im Array indexArr beschrieben ist, d.h.
     das Element i aus dem Array orgArr wird an die Stelle
     indexArr[i] im Array permutSeq kopiert.
\param[in]    orgArr
\param[out]   permutSeq,
\param[in]    indexArr
*/
void makeAircraftPermutationAddr
   (
   vector<Aircraft>& orgArr,
   vector<Aircraft*>& permutSeq,
   const int indexArr[]
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "makeAircraftPermutationAddr", "");
   for (int i=0; i < static_cast<int>(orgArr.size()); ++i)
   {      
      permutSeq[indexArr[i] ] = &(orgArr[i]);
   }
} // makeAircraftPermutation



/**
Die Funktion ermittelt die optimale Reihenfolge für die Flugzeuge
im Array elems  Für das erste Flugzeug in elems ist
vorg der Vorgänger
Bewertungsfunktion func und expo sind Parameter.

\param[in]   elems  (Array)
\param[out]  bestSeq (Array)
\param[in]   vorg
\param[in]   func, evaluation function
\param]in]   expo, exponent maybe used by func
\return      true, wenn erfolgreich
*/
bool getBestSequenceFunc
   (
   const vector<Aircraft>& elems,
   vector<Aircraft>& bestSeq,
   const Aircraft& vorg,
   T_EvalFunc func,
   double expo
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequenceFunc", "");
   vector<Aircraft> permutAcft = elems;

   // Array mit den Zahlen 0, 1,... (Anzahl Gäste -1) erstellen
   int* indexArr = new int[elems.size()];
   int nFak=1;
   for (int i= 0; i < static_cast<int>(elems.size()); ++i)
   {
      indexArr[i] = i;
      nFak *= (i+1);
   }

   double bestseqValue = DBL_MAX; // ganz schlechter Wert
   for (int i= 0; i < nFak; ++i)
   {
      makeAircraftPermutation(elems, permutAcft, indexArr);
      double seqValue = evaluateSequenceFunc(permutAcft, vorg, func, expo);
      if (seqValue < bestseqValue)
      {
         bestSeq = permutAcft;
         bestseqValue = seqValue;
      }
     next_permutation(&indexArr[0], &(indexArr[elems.size()] ));
   }// for i

      TRACE("BestSeqis ");
      for (int i=0; i < static_cast<int>(bestSeq.size()); ++i) {
         TRACE("  " << bestSeq[i]);
      }
      TRACE("Quality Value: " << bestseqValue);

      delete[] indexArr;
      return true;

}// getBestSequenceFunc


/**
Die Funktion ermittelt die optimale Reihenfolge für die Flugzeuge
im Array elems  Für das erste Flugzeug in elems ist
vorg der Vorgänger
Bewertungsfunktion ist in qualiFuncAcft spezifiziert. 

\param[in]   elems  (Array)
\param[out]  bestSeq (Array)
\param[in]   vorg
\param[in]   qualiFuncAcft Bewertungsfunktionen
\return      quality value of best sequence
*/
double getBestSequenceQualiClass
   (
   vector<Aircraft>& elems,
   vector<Aircraft>& bestSeq,
   const Aircraft& vorg,
   QualityFuncAircraft& qualiFuncAcft
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequenceQualiClass", "");

   vector<Aircraft*> permutAcft;
   for (int i=0; i < static_cast<int>(elems.size()); ++i)
   {
      permutAcft.push_back(&(elems[i]));
   }

   // Array mit den Zahlen 0, 1,... (Anzahl Gäste -1) erstellen
   int* indexArr = new int[elems.size()];
   int nFak=1;
   for (int i= 0; i < static_cast<int>(elems.size()); ++i)
   {
      indexArr[i] = i;
      nFak *= (i+1);
   }

   double bestseqValue = DBL_MAX; // ganz schlechter Wert
   for (int i= 0; i < nFak; ++i)
   {
      makeAircraftPermutationAddr(elems, permutAcft, indexArr);
      double seqValue = qualiFuncAcft.CalcValueOfSeq1Pred(permutAcft, vorg);

      if (seqValue < bestseqValue)
      {
         bestSeq.clear();
         for (int i= 0; i < static_cast<int>(elems.size()); ++i)
         {
            bestSeq.push_back(* (permutAcft[i]) );
         }
         bestseqValue = seqValue;
      }
     next_permutation(&indexArr[0], &(indexArr[elems.size()] ));
   }// for i

      TRACE("BestSeqis ");
      for (int i=0; i < static_cast<int>(bestSeq.size()); ++i) {
         TRACE("  " << bestSeq[i]);
      }
      TRACE("Quality Value: " << bestseqValue);

      delete[] indexArr;
      return bestseqValue;

}// getBestSequenceQualiClass


/** Die übergebene Sequenz wird ausgegeben.
*/
void printSequenceTo
   (
   ostream& str,
   const vector<Aircraft>& seq
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "printSequenceTo", "");
   for (int i=0; i < static_cast<int>(seq.size()); ++i)
   {
      str << seq[i];
      str << "\n";
   }
}
