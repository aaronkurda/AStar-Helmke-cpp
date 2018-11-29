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

#ifndef QualityFuncStatistics_HEADER
#define QualityFuncStatistics_HEADER



/*!
\file QualityFuncStatistics.h
\author Hartmut Helmke (HHe)

\date 2010-09-05 HHe: Initial version.

*/


#include <fstream>
/*======================*
  declaration of QualityFuncStatistics
 *======================*/

/*! 
Class for counting the number of evaluated nodes
*/
class QualityFuncStatistics
{
public:
   QualityFuncStatistics();
   virtual ~QualityFuncStatistics();

   int GetEvalNodeCnt()    const {return mi_evalNodeCnt;}
   void ResetEvalNodeCnt() {mi_evalNodeCnt=0;}
   void IncEvalNodeCnt() {++mi_evalNodeCnt;}


private:
   //! number of times the quality function was called for one node
   int mi_evalNodeCnt;

   friend std::ostream& operator<<(std::ostream& str, const QualityFuncStatistics& qf);

};//QualityFuncStatistics



#endif /* QualityFuncStatistics_HEADER */
