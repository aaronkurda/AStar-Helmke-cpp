//  Project       : 4D_CARMA,
//  Workpackage   : Scheduler (Test)
//  
//               Copyright (C) 2008, 2009, 2010, ...
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
//  without the prior written consent of the German Aerospace Center.     

/*!
\file SearchAircraftTest.h

\author Hartmut Helmke (HHe)

\date 2010-09-05 HHe: Initial version for WS 2010/11

*/ 

#ifndef SearchAircraftTest_HEADER
#define SearchAircraftTest_HEADER

bool testEvaluateSequenceAcft();
bool testEvalAcftSeqByDiffWeights();
bool testTakeSelectAcftQualiEarly();
bool testTakeSelectAcftSameQuali();
bool testTakeSelectAcftQualiEarlyLate();

bool testAircraft9WithQualityClass();
bool testAircraft9WithQualityClassEarlyLate();
bool testAircraft9WithQualityClassEarly();
#endif
