/* 18-November-2017
Copyright: H.Helmke,
Header-Datei fuer den A*-Algorithmus
*/

#ifndef AStarAcft_HEADER
#define AStarAcft_HEADER

#include <vector>

#include "QualityFuncAircraft.h"
#include "Aircraft.h"

class AStarAcft
{
public:
   AStarAcft(const QualityFuncAircraft& a_qualiFunc);
   bool getBestSequence(
      const std::vector<Aircraft>& inpAcftVect, std::vector<Aircraft>& bestAcftSeq);
   double getEvalForBestSeq() const  { return md_bestSeqValue;}

private:
   double md_bestSeqValue;
   QualityFuncAircraft m_qualiFunc;

};


#endif /* AStarAcft_HEADER */
