/* 18-September-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung und TakeSelect-Algorithmus
   Header-datei der Klasse TakeSelect
*/

#ifndef TakeSelect_HEADER
#define TakeSelect_HEADER



#include <vector> /*STL*/
using namespace std;

#include "TakeSelectSearchParam.h" //TakeSelectSearchParam
#include "Guest.h"
#include "GuestArray.h"
#include "Aircraft.h"
#include <vector>
#include "QualityFuncStatistics.h"
#include "EvalSequence.h"

/*! 
Die Klasse TakeSelect implementiert den TakeSelect-Algorithmus
*/
class TakeSelect
{
public:
   //! constructor.
   TakeSelect(const TakeSelectSearchParam& ar_searchParam);
   //! destructor.
   ~TakeSelect();
      //! Interface of the  Take-Select-algorithm
   double GetBestSeq(const GuestArray& arc_inputSequence,
                     GuestArray& arc_outputSequence); 
      //! Interface of the  Take-Select-algorithm
   double GetBestSeqFunc(const GuestArray& arc_inputSequence,
                     GuestArray& arc_outputSequence, T_EvalFunc f, double expo); 
   double GetBestQualiClass(const vector<Aircraft>& arc_inputSequence, 
      vector<Aircraft>& arc_outputSequence, class QualityFuncAircraft& ar_qualiFunc);

   const  QualityFuncStatistics& GetStatistics() const {return  m_statistics;} 
   QualityFuncStatistics& GetStatistics() {return  m_statistics;} 

private:
   //! e.g. take and select count for sliding window
   TakeSelectSearchParam m_searchParam;
   QualityFuncStatistics m_statistics;

   //! forbidden copy constructor
   TakeSelect(const TakeSelect& astar);
   //! forbidden operator
   TakeSelect& operator=(const TakeSelect& astar);

};//TakeSelect


#endif /* TakeSelect_HEADER */

