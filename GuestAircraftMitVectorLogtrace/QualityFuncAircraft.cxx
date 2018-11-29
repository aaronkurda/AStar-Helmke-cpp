//  Project       : 4D_CARMA,
//  Workpackage   : Scheduler
//  
//               Copyright (C) 2010, ...
//            Deutsches Zentrum fuer Luft- und Raumfahrt e.V.             
//                       German Aerospace Center                          
//                                                                        
//       Institut fuer Flugfuehrung/Institute of Flight Guidance          
//              Tel. +49 531 295 2501, Fax: +49 531 295 2550
//                 WWW: http://www.bs.dlr.de/FL/                       
//                                                                        
//  These coded instructions, statements, and computer programs contain   
//  unpublished proprietary information of the German Aerospace Center    
//  and are protected by the copyright law. They may not be disclosed to  
//  third parties or copied or duplicated in any form,in whole or in part,
//  without the prior written consent of the German Aerospace Center      

/*!
\file QualityFuncAircraft.cxx
\brief Definition of Class QualityFuncAircraft.
 
This file contains the constructor, destructor and method definitions of Class QualityFuncAircraft.
*/

/*!
\file QualityFuncAircraft.cxx
\author Hartmut Helmke (HHe)

\date 2018-10-31 HHe:  QualityFuncAircraft::CalcValueOfSeq: output to g_outFile replace by TRACE

*/

#include "QualityFuncAircraft.h"
#include <algorithm> // wg. max

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;

#ifdef LOGTRACE_DEBUG
   static const char NAMESP_LT[]="Scheduling";
   static const char CLASS_LT[]="EvaluateSeq";
#endif

   extern ofstream g_outFile;

/*!
Default-Constructor
*/
QualityFuncAircraft::QualityFuncAircraft() 
{
   md_earlTimeWeight = 1.0/3.0;
   md_lateTimeWeight = 1.0/3.0;
   md_lastScheduleWeight = 1.0/3.0;
}

/*!
Destructor
*/
QualityFuncAircraft::~QualityFuncAircraft()
{
}


void QualityFuncAircraft::SetCriteriaWeights
   (
    double earlTimeWeight, 
    double lateTimeWeight, 
    double lastScheduleWeight
    )
{
   double factor = earlTimeWeight + lateTimeWeight + lastScheduleWeight;
   INVARIANT(factor > 0, "scale factor <= 0");
   md_earlTimeWeight = earlTimeWeight / factor;
   md_lateTimeWeight = lateTimeWeight / factor;
   md_lastScheduleWeight = lastScheduleWeight / factor;
}

/** Ermittlung der erforderlichen Mindestseparation zwischen
dem Aircraft first und dem Nachfolger follower.
*/
static inline Zeit calcSep
   (
   const Aircraft& first,
   const Aircraft& follower
   )
{
/*   Heavy, Medium, Light, A380} */
static const Zeit sepMatrix[4][4] = {{100, 125, 150, 100},
                  {75, 75, 125, 75},
                  {75,75,75,75},
                  {125, 150, 200, 100}};
   return sepMatrix[follower.getWeightClass()][first.getWeightClass()];
}

/** Quadratische Bewertung  der Abweichung von targetTime von
der Referenzzeit refTime, wenn targetTime größer ist.
*/
static inline double evaluateTimeSquareAcft
   (
   int refTime,
   int targetTime
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? diff*diff : 0;
}

/** Gewichtsfunktion zur Berücksichtigung der Abweichung
von der spätesten Zeit, der Abweichung von der frühest möglichen
Zeit und von der zuletzt geplanten Zeit.
*/
double QualityFuncAircraft::evaluateTargetTime
   (
   const Aircraft& arc_acft
   ) const
{
   Zeit targTime = arc_acft.getTargetTime();
   double seqValue = md_earlTimeWeight *
      evaluateTimeSquareAcft(arc_acft.getEarliestTime(), targTime);
   seqValue += md_lateTimeWeight * 
      evaluateTimeSquareAcft(arc_acft.getLatestTime(), targTime);
   seqValue += md_lastScheduleWeight * 
      evaluateTimeSquareAcft(arc_acft.getlastSchedule(), targTime);
   return seqValue;
}


/** Bewertung des Aircraft elem, wobei seine
Vorgänger in arc_prev gegeben sind.
arc_prev[arc_prev.size() - 1] ist somit
der unmittelbare Vorgänger von elem
In elem wird die Zielzeit eingetragen, wenn die Mindestseparation 
zu den Vorgänger beachtet wird.

\param     elem
\param[in] arc_prev
*/
double 
QualityFuncAircraft::CalcOneQValueOfElem
   (
   Aircraft* elem, 
   const std::vector<Aircraft*>& arc_prev
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "CalcOneQValueOfElem", "");
   INVARIANT(arc_prev.size() >= 1 && arc_prev[0] != 0,
      "No previous element set");
   const Aircraft& vorgElem = *(arc_prev[0]);
   Zeit targTime = max(elem->getEarliestTime(),
      vorgElem.getTargetTime() + calcSep(*elem, vorgElem));
   elem->setTargetTime(targTime);
   return evaluateTargetTime(*elem);
}

/** Bewertung der Sequenz, wie sie in arc_seq gegeben ist
und die Vorgängerflugzeuge durch den Vektor arc_prev
bestimmt sind. arc_prev[arc_prev.size() - 1] ist somit
der unmittelbare Vorgänger von arc_seq[0].
In arc_seq wird die Zielzeit eingetragen, wenn diese
Reihenfolge eingehalten wird und die Mindestseparation gewählt 
wird.

\param     arc_seq  
\param[in] arc_prev
*/
double QualityFuncAircraft::CalcValueOfSeq
   (
   std::vector<Aircraft*>& arc_seq,
   const std::vector<const Aircraft*>& arc_prev
   ) 
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "CalcValueOfSeq", "");
   INVARIANT(arc_prev.size() >= 1 && arc_prev[0] != 0,
      "No previous element set");
   TRACE("Evaluation of arc_seq");
   for (int i=0; i < static_cast<int>(arc_seq.size()); ++i) {
      TRACE("  " << *(arc_seq[i]) );
   }

   const Aircraft& vorgElem = *(arc_prev[0]);
   Zeit targTime = max(arc_seq[0]->getEarliestTime(),
      vorgElem.getTargetTime() + calcSep(*(arc_seq[0]), vorgElem));

   arc_seq[0]->setTargetTime(targTime);
   double seqValue = evaluateTargetTime(*(arc_seq[0]));
   TRACE(arc_seq[0]->getCallsign() << " " << targTime << " " << arc_seq[0]->getEarliestTime());
   TRACE(arc_seq[0]->getCallsign() << " " << targTime << " " << arc_seq[0]->getLatestTime());
   TRACE(arc_seq[0]->getCallsign() << " " << targTime << " " << arc_seq[0]->getlastSchedule());

   for (int i=1; i < static_cast<int>(arc_seq.size()); ++i)
   {
      // Maximum aus Aufstehzeit und Verlassen durch Vorgänger
      targTime = max(arc_seq[i]->getEarliestTime(),
         arc_seq[i-1]->getTargetTime() + 
         calcSep(*(arc_seq[i]), *(arc_seq[i-1])));
      arc_seq[i]->setTargetTime(targTime);
      seqValue += evaluateTargetTime(*(arc_seq[i]));
      TRACE(arc_seq[i]->getCallsign() << " " << targTime << " " << arc_seq[i]->getEarliestTime())
         TRACE(arc_seq[i]->getCallsign() << " " << targTime << " " << arc_seq[i]->getLatestTime());
      TRACE(arc_seq[i]->getCallsign() << " " << targTime << " " << arc_seq[i]->getlastSchedule());

   }// for i

   TRACE("   Quality is " << seqValue);

   return seqValue;
}
