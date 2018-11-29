//  Project       : 4D_CARMA,
//  Workpackage   : Scheduler
//  
//               Copyright (C) 2008
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

#ifndef QualityFuncBase_HEADER
#define QualityFuncBase_HEADER


/*!
\file QualityFuncBase.h
\author Hartmut Helmke (HHe)
*/

#include "QualityFuncStatistics.h"
#include <vector>

/*======================*
  declaration of QualityFuncBase
 *======================*/

template <class T>
class QualityFuncBase
{
public:
   QualityFuncBase();    
   //! destructor.
   virtual ~QualityFuncBase();

   /*! quality function for calculation the quality value of elem
   considering the predecessors of elem in vector prev */
   virtual double CalcOneQValueOfElem(T* elem, const std::vector<T*>& prev) = 0; 
   /** see CalcOneQValueOfElem where minValue is an output parameter.
   It is the minimal value which can be assigned to elem with the same predecessors
   (perhaps another runway or later time) */
   virtual double CalcValueOfElem(T* elem, const std::vector<T*>& prev, double& minValue); 

   /*! evaluating the whole sequence in  arc_seq, with prev[size-1] the predecessor of the
       first element. */
   virtual double CalcValueOfSeq(std::vector<T*>& arc_seq,  const std::vector<const T*>& prev) = 0;
   /*! evaluating the whole sequence in  arc_seq, with pred as the predecessor of the
       first element. */
   virtual double CalcValueOfSeq1Pred(std::vector<T*>& arc_seq, const T& pred);

   const QualityFuncStatistics&  GetStatisticsConst()const {return m_qualityFuncStatistics;}
   QualityFuncStatistics&  GetStatistics(){return m_qualityFuncStatistics;}

private:
   QualityFuncStatistics m_qualityFuncStatistics;


};//QualityFuncBase

#include "QualityFuncBase.cxx"


#endif /* QualityFuncBase_HEADER */
