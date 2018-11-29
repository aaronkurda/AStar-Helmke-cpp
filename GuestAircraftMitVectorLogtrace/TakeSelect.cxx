/* 18-September-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung und TakeSelect-Algorithmus
   Quellcodedatei der Klasse TakeSelect
*/

#include "TakeSelect.h"

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;

#ifdef LOGTRACE_DEBUG
static const char  LOGTRACE_TakeSelectM[] = "Scheduling";
static const char  LOGTRACE_TakeSelectCl[] = "TakeSelect";
#endif

#include "EvalSequence.h"
#include "QualityFuncAircraft.h"
#include "Aircraft.h"
#include "EvalSeqAircraft.h"

/*!
detailed description of constructor.
*/
TakeSelect::TakeSelect
(
    const TakeSelectSearchParam& ar_searchParam
 ):
    m_searchParam(ar_searchParam)
{
   LOG_CONSTRUCTOR(LOGTRACE_TakeSelectM,LOGTRACE_TakeSelectCl,"");
   m_statistics.ResetEvalNodeCnt();
}//TakeSelect;


/*!
destructor.
*/
TakeSelect::~TakeSelect()
{
   LOG_DESTRUCTOR(LOGTRACE_TakeSelectM,LOGTRACE_TakeSelectCl);
   //TRACE_ID(ALWAYS_ID, "TakeSelect Statistics:\n   " << m_statistics);
}//~TakeSelect





/*!
Die Elemente in der Eingangssequenz arc_inputSequence werden mit dem 
Take/Select-Algorithmus optimiert.
Es werden jeweils die nächsten m_searchParam.GetTakeCnt() Elemente arc_inputSequence.
Durch vollständige Permutation dieser Elemente wird die beste Sequenz bestimmt.
Hiervon werden  m_searchParam.GetSelectCnt() Elemente ausgewählt und in
arc_outputSequence eingefügt.
Die verbleibenden  (m_searchParam.GetTakeCnt() -  m_searchParam.GetSelectCnt())
Elemente werden um nächsten Elemente aus arc_inputSequence ergänzt und
es wird wieder das Optimum durch vollständige Permutation bestimmt.

\param[in] arc_inputSequence Sequence which is permutated optimized by TakeSelect
\param[out] arc_outputSequence: best sequence, determined by Take Select
\return double: quality value of best sequence
*/
double TakeSelect::GetBestSeq
   (
   const GuestArray& arc_inputSequence,
   GuestArray& arc_outputSequence
   )
{
   LOG_METHOD(LOGTRACE_TakeSelectM,LOGTRACE_TakeSelectCl,"GetBestSeq","");

   arc_outputSequence.clear();

   // Kopie erstellen, da verändert wird.
   GuestArray inputSequence(arc_inputSequence.size());
   inputSequence=arc_inputSequence;

   // wird gebraucht, da hier auch ein Dummy-Gast eingefügt wird.
   Guest vorgaengerGast;

   int scheduledCnt=0;//Anzahl der Eingangssequenz, die bereits betrachtet einsortiert sind
   while (scheduledCnt < arc_inputSequence.numberOfGuests())
   {
      // Eine temporäre Sequenz mit select Elementen wird belegt.
      GuestArray tmpInputSequence(m_searchParam.GetTakeCnt());
      int i=0;
      while  ((i < m_searchParam.GetTakeCnt()) && 
         (tmpInputSequence.size() < inputSequence.size() ))
      {
         TRACE("added to tmpInputSequence : " << inputSequence.getGuest(i).getGuestName());
         tmpInputSequence.storeGuest(inputSequence.getGuest(i));
         ++i;
      } // while  ((i < m_searchParam.GetTakeCnt()) &&  ...

      // tmpInputSequence wird nun optimiert.
      GuestArray tmpOutputSequence(m_searchParam.GetTakeCnt());
      getBestSequence(tmpInputSequence, tmpOutputSequence, vorgaengerGast);

      // Einige Elemente (take Stück) aus tmpOutputSequence werden in die endgültige
      // Ausgangssequenz übernommen

      int selectCnt = (inputSequence.size() <= m_searchParam.GetTakeCnt() ) ? 
         inputSequence.numberOfGuests() :  m_searchParam.GetSelectCnt();

      for (int i=0; i < selectCnt; ++i)
      {
         TRACE("added to arc_outputSequence : " << 
            tmpOutputSequence.getGuest(i).getGuestName());
         arc_outputSequence.storeGuest(tmpOutputSequence.getGuest(i));
         inputSequence.deleteElement(tmpOutputSequence.getGuestName(i));
      }
      scheduledCnt += selectCnt;

      // Das letzte Element der bereits vorhandenen Ausgabesequenz wird 
      // als Vorgänger der nächsten Iteration verwendet. 
      vorgaengerGast =  arc_outputSequence.
         getGuest(arc_outputSequence.numberOfGuests() - 1);

   } // while ((i < m_searchParam.GetTakeCnt()) &&  ...

   Guest dummy;
   return evaluateSequence(arc_outputSequence,  dummy);

}//TakeSelect::GetBestSeq




/*!
Die Elemente in der Eingangssequenz arc_inputSequence werden mit dem 
Take/Select-Algorithmus optimiert.
Es werden jeweils die nächsten m_searchParam.GetTakeCnt() Elemente arc_inputSequence.
Durch vollständige Permutation dieser Elemente wird die beste Sequenz bestimmt.
Hiervon werden  m_searchParam.GetSelectCnt() Elemente ausgewählt und in
arc_outputSequence eingefügt.
Die verbleibenden  (m_searchParam.GetTakeCnt() -  m_searchParam.GetSelectCnt())
Elemente werden um nächsten Elemente aus arc_inputSequence ergänzt und
es wird wieder das Optimum durch vollständige Permutation bestimmt.

\param[in] arc_inputSequence Sequence which is permutated optimized by TakeSelect
\param[out] arc_outputSequence: best sequence, determined by Take Select
\param[in] func, used evaluations function
\param[in] exp, exponent maybe used by func
\return double: quality value of best sequence
*/
double TakeSelect::GetBestSeqFunc
   (
   const GuestArray& arc_inputSequence,
   GuestArray& arc_outputSequence,
   T_EvalFunc func,
   double expo
   )
{
   LOG_METHOD(LOGTRACE_TakeSelectM,LOGTRACE_TakeSelectCl,"GetBestSeqQuad","");

   arc_outputSequence.clear();

   // Kopie erstellen, da verändert wird.
   GuestArray inputSequence(arc_inputSequence.size());
   inputSequence=arc_inputSequence;

   // wird gebraucht, da hier auch ein Dummy-Gast eingefügt wird.
   Guest vorgaengerGast;

   int scheduledCnt=0;//Anzahl der Eingangssequenz, die bereits betrachtet einsortiert sind
   while (scheduledCnt < arc_inputSequence.numberOfGuests())
   {
      // Eine temporäre Sequenz mit select Elementen wird belegt.
      GuestArray tmpInputSequence(m_searchParam.GetTakeCnt());
      int i=0;
      while  ((i < m_searchParam.GetTakeCnt()) && 
         (tmpInputSequence.size() < inputSequence.size() ))
      {
         TRACE("added to tmpInputSequence : " << inputSequence.getGuest(i).getGuestName());
         tmpInputSequence.storeGuest(inputSequence.getGuest(i));
         ++i;
      } // while  ((i < m_searchParam.GetTakeCnt()) &&  ...

      // tmpInputSequence wird nun optimiert.
      GuestArray tmpOutputSequence(m_searchParam.GetTakeCnt());
      getBestSequenceFunc(tmpInputSequence, tmpOutputSequence, vorgaengerGast, func, expo);

      // Einige Elemente (take Stück) aus tmpOutputSequence werden in die endgültige
      // Ausgangssequenz übernommen

      int selectCnt = (inputSequence.size() <= m_searchParam.GetTakeCnt() ) ? 
         inputSequence.numberOfGuests() :  m_searchParam.GetSelectCnt();

      for (int i=0; i < selectCnt; ++i)
      {
         TRACE("added to arc_outputSequence : " << 
            tmpOutputSequence.getGuest(i).getGuestName());
         arc_outputSequence.storeGuest(tmpOutputSequence.getGuest(i));
         inputSequence.deleteElement(tmpOutputSequence.getGuestName(i));
      }
      scheduledCnt += selectCnt;

      // Das letzte Element der bereits vorhandenen Ausgabesequenz wird 
      // als Vorgänger der nächsten Iteration verwendet. 
      vorgaengerGast =  arc_outputSequence.
         getGuest(arc_outputSequence.numberOfGuests() - 1);

   } // while ((i < m_searchParam.GetTakeCnt()) &&  ...

   Guest dummy;
   return evaluateSequenceFunc(arc_outputSequence,  dummy, func, expo);

}//TakeSelect::GetBestSeqFunc



void DeleteFromSequence
   (
   vector<Aircraft>& ac_inputSequence,
   const Aircraft& arc_elem
   )
{
   vector<Aircraft>::iterator iter = ac_inputSequence.begin();
   while (iter != ac_inputSequence.end() &&
      (*iter).getIdentifier() != arc_elem.getIdentifier())
   {
      ++iter;
   } // while
   if (iter != ac_inputSequence.end())
   {
      ac_inputSequence.erase(iter);
   }
}



/*!
Die Elemente in der Eingangssequenz arc_inputSequence werden mit dem 
Take/Select-Algorithmus optimiert.
Es werden jeweils die nächsten m_searchParam.GetTakeCnt() Elemente arc_inputSequence.
Durch vollständige Permutation dieser Elemente wird die beste Sequenz bestimmt.
Hiervon werden  m_searchParam.GetSelectCnt() Elemente ausgewählt und in
arc_outputSequence eingefügt.
Die verbleibenden  (m_searchParam.GetTakeCnt() -  m_searchParam.GetSelectCnt())
Elemente werden um nächsten Elemente aus arc_inputSequence ergänzt und
es wird wieder das Optimum durch vollständige Permutation bestimmt.

\param[in] arc_inputSequence Sequence which is permutated optimized by TakeSelect
\param[out] arc_outputSequence: best sequence, determined by Take Select
\param[in]  ar_qualiFunc  used evaluations function
\return double: quality value of best sequence
*/
double TakeSelect::GetBestQualiClass
   (
   const vector<Aircraft>& arc_inputSequence, 
   vector<Aircraft>& arc_outputSequence,
   QualityFuncAircraft& ar_qualiFunc
   )
{
   LOG_METHOD(LOGTRACE_TakeSelectM,LOGTRACE_TakeSelectCl,"GetBestQualiClass","");

   arc_outputSequence.clear();

   // Kopie erstellen, da verändert wird.
   vector<Aircraft> inputSequence(arc_inputSequence);

   // wird gebraucht, da hier auch ein Dummy-Gast eingefügt wird.
   Aircraft  predAcft;

   int scheduledCnt=0;//Anzahl der Eingangssequenz, die bereits betrachtet einsortiert sind
   while (scheduledCnt < static_cast<int>(arc_inputSequence.size()) )
   {
      // Eine temporäre Sequenz mit select Elementen wird belegt.
      vector<Aircraft> tmpInputSequence;
      int i=0;
      while  ((i < m_searchParam.GetTakeCnt()) && 
         (tmpInputSequence.size() < inputSequence.size() ))
      {
         TRACE("added to tmpInputSequence : " << inputSequence[i].getIdentifier() );
         tmpInputSequence.push_back(inputSequence[i]);
         ++i;
      } // while  ((i < m_searchParam.GetTakeCnt()) &&  ...

      // tmpInputSequence wird nun optimiert.
      vector<Aircraft> tmpOutputSequence(m_searchParam.GetTakeCnt());
      getBestSequenceQualiClass(tmpInputSequence, tmpOutputSequence, predAcft, ar_qualiFunc);

      // Einige Elemente (take Stück) aus tmpOutputSequence werden in die endgültige
      // Ausgangssequenz übernommen
      int selectCnt = (static_cast<int>(inputSequence.size()) <= m_searchParam.GetTakeCnt() ) ? 
         static_cast<int>(inputSequence.size()) :  m_searchParam.GetSelectCnt();

      for (int i=0; i < selectCnt; ++i)
      {
         TRACE("added to arc_outputSequence : " << 
            tmpOutputSequence[i].getIdentifier() );
         arc_outputSequence.push_back(tmpOutputSequence[i]);
         DeleteFromSequence(inputSequence, tmpOutputSequence[i]);
      }
      scheduledCnt += selectCnt;

      // Das letzte Element der bereits vorhandenen Ausgabesequenz wird 
      // als Vorgänger der nächsten Iteration verwendet. 
      predAcft =  arc_outputSequence[arc_outputSequence.size() - 1];

   } // while ((i < m_searchParam.GetTakeCnt()) &&  ...

   Aircraft dummy;
   vector<Aircraft*> outSeqPtr;
   for (int i=0; i < static_cast<int>(arc_outputSequence.size()); ++i)
   {
      outSeqPtr.push_back(&(arc_outputSequence[i]));
   }
   return ar_qualiFunc.CalcValueOfSeq1Pred(outSeqPtr,  dummy);

}//TakeSelect::GetBestQualiClass



