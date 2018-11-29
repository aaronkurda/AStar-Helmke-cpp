/* 18-November-2017
   Copyright: H.Helmke, 
   Headerdatei der Tests fuer den A*-Algorithmus
*/

#ifndef TESTASTAR_ACFT_HEADER
#define TESTASTAR_ACFT_HEADER

#include <fstream>
#include <vector>
#include <string>
#include "Aircraft.h"

bool testOptiWithAStarAcft();

bool test9AircraftWithDifferentWeightedCriteria();
bool test5to9SimpleAcftFiles();
bool test10to16SimpleAcftFiles();
bool test5to15DifficultAcftFiles();

void printSequenceTo(std::ostream& str,
                     const std::vector<Aircraft>& seq,
                     double eval);


bool testSeqValueAndSequenceAcft(
   std::string inpFileName,
   const std::vector<std::string>& ar_expectedSeq,
   double ad_expValue,
   const class QualityFuncAircraft& ar_qualiFunc);

#endif /* TESTASTAR_ACFT_HEADER */
