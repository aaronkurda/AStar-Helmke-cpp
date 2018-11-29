//  Project       : 4D_CARMA,
//  Workpackage   : Scheduler
//  
//               Copyright (C) 2008-2009
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
\file QualityFuncBase.cxx
\brief Definition of Class QualityFuncBase.
 
This file contains the constructor, destructor and method definitions of Class QualityFuncBase.
*/

/*!
\file QualityFuncBase.cxx
\author Hartmut Helmke (HHe)

*/

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;


/*!
Default-Constructor
*/
template <class T>
QualityFuncBase<T>::QualityFuncBase() 
{
}


/*!
Destructor
*/
template <class T>
QualityFuncBase<T>::~QualityFuncBase()
{
}



template <class T>
double 
QualityFuncBase<T>::CalcValueOfSeq1Pred(std::vector<T*>& arc_seq, const T& ar_prev)
{
   std::vector<const T*> predArr(1);
   predArr[0] = const_cast<T*>(& ar_prev);
   return CalcValueOfSeq(arc_seq, predArr); 
}

template <class T>
double 
QualityFuncBase<T>::CalcValueOfElem(T* elem, const std::vector<T*>& prev, double& minValue) 
{
   return (minValue = CalcOneQValueOfElem(elem, prev));
}