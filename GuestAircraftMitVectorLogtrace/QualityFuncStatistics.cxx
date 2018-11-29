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
\file QualityFuncStatistics.cxx
\brief Definition of Class QualityFuncStatistics.
 
This file contains the constructor, destructor and method definitions of Class QualityFuncStatistics.
*/

/*!
\file QualityFuncStatistics.cxx
\author Hartmut Helmke (HHe)

\date 2010-09-05 HHe: Initial version
*/

#include "QualityFuncStatistics.h"

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;

/*!
Default-Constructor
*/
QualityFuncStatistics::QualityFuncStatistics() 
{
   ResetEvalNodeCnt();
}

QualityFuncStatistics::~QualityFuncStatistics() 
{
}


std::ostream& operator<<
   (
   std::ostream& str, 
   const QualityFuncStatistics& qf
   )
{
   str << "QualiFunc-Calls: " << qf.GetEvalNodeCnt();
   return str;
}
