/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei der Funktionalität der Sequenzplanung und -bewertung
*/
#ifndef EvalSeqAircraft_HEADER
#define EvalSeqAircraft_HEADER

#include <vector>
#include <fstream>
#include "Aircraft.h"

typedef double (*T_EvalFunc)(int refTime, int targetTime, double expo);
double evaluateTimeSquareAcft(int refTime,int targetTime, double);

bool getBestSequenceFunc(const std::vector<Aircraft>& elems, std::vector<Aircraft>& bestSeq,
      const Aircraft& vorg, T_EvalFunc func, double expo);
double getBestSequenceQualiClass(std::vector<Aircraft>& elems,
	std::vector<Aircraft>& bestSeq, const Aircraft& vorg,
   class QualityFuncAircraft& qualiFuncAcft);
void printSequenceTo(std::ostream& str, const std::vector<Aircraft>& seq);

double evaluateSequenceFunc(std::vector<Aircraft>& seq,
   const Aircraft&  vorgElem,
   T_EvalFunc func, double expo);

#endif /* EvalSeqAircraft_HEADER */
