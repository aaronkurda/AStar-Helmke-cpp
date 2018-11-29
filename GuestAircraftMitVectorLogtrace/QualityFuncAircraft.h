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

#ifndef QualityFuncAircraft_HEADER
#define QualityFuncAircraft_HEADER




/*!
\file QualityFuncAircraft.h
\author Hartmut Helmke (HHe)

*/

#include "QualityFuncBase.h"
#include <vector>
#include "Aircraft.h"

/*======================*
  declaration of QualityFuncAircraft
 *======================*/
class QualityFuncAircraft: public QualityFuncBase<Aircraft>
{
public:
   QualityFuncAircraft();    
   virtual ~QualityFuncAircraft();
   //! Setting the weights of the different criteria
   void SetCriteriaWeights(double earlTimeWeight, double lateTimeWeight, 
      double lastScheduleWeight);
   /*! quality function for calculation the quality value of elem
   considering the predecessors of elem in vector prev */
   virtual double CalcOneQValueOfElem(Aircraft* elem, const std::vector<Aircraft*>& prev); 
   /*! evaluating the whole sequence in  arc_seq, with prev[size-1] the predecessor of the
       first element. */
   virtual double CalcValueOfSeq(std::vector<Aircraft*>& arc_seq,  const std::vector<const Aircraft*>& prev);
private:
   double evaluateTargetTime(const Aircraft& arc_acft) const;
    // Attributes
   double md_earlTimeWeight;
   double md_lateTimeWeight;
   double md_lastScheduleWeight;

};//QualityFuncAircraft

#endif /* QualityFuncAircraft_HEADER */
