//  Project       : 4D_CARMA,
//  Workpackage   : libaero
//  
//               Copyright (C) 2000-2009, 2010, ...
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
\file zeitmess.cxx
\brief Utilities for runtime measurement

\author Prof. Dr. Hartmut Helmke (HHe)
*/


/**
\date 2010-05-17 HHe: new makros  ABS_ZEITMESS_SCHL_ID and  ZEITMESS_SCHL_ID
\date 2010-08-28 HHe: replaced char* by class string
*/

#include "zeitmess.h"
#include <cstring>
using namespace std;


/*
##############################################################################

# Funktions Name   : K_TIMER::K_TIMER()
#
# Beschreibung     : Konstruktor
#
# Input Parameter  :  str  // wird bei der Ausgabe verwendet
#
# Output Parameter : keine
#
# Bemerkungen      : keine
#
##############################################################################
*/
K_TIMER::K_TIMER(char * str)
{ 
ges_zeit=0.0; anz_timer_aufrufe=0; max_time=0.0; anf_zeit=0;
name = string(str);
} // K_TIMER::K_TIMER()

K_TIMER::~K_TIMER()   
{
}

K_R_TIMER::~K_R_TIMER() 
{
;
}


#  ifndef _MSC_VER
void Sleep (int delay)
   {
// Die Funktion Sleep wartet fuer die angegebene Zeit. Damit nicht
// nur ganzzahlige Werte moeglich sind, wird die Wartezeit durch
// select erreicht.

   int    sec = delay / 1000;
   double del = (float)delay / 1000.;
   struct timeval wait;

   wait.tv_sec  = sec;
   wait.tv_usec = (int) ((del - (double)sec) * 1000000);
   select (0, 0, 0, 0, &wait);
   return;
}
#endif
