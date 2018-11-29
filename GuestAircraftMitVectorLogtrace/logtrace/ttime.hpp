/*
//  Project       : Generic Trajectory Generator (GenTraG),
                    4D-Planner (4DP),
                    Intelligent Pilot Assistant (IPA)
//  Workpackage   : Basic Utility Files
//  
//  Author        : Dipl.-Inform Frank Hoeppner (OOW)
//

  Description   : header of function module ttime
                  for documentation see ttime.tex
                  Source Documentation Handbook: tutorial-UTIL
  History       : see source file

//  Environment   : Source Documentation Handbook: tutorial-UTIL
//  Compiler      : gcc version 3.3.3
                    Microsoft Visual Studio Professional 6.1 and 7.0
//
//
//
//  Comment       : 
//  This file was generated automatically. DO NOT EDIT.
//  Ruleviol.     :  
//                    
//
//
//  Copyright     : Copyright (C) 1999-2000 Frank Hoeppner
//                                2000-2005 DLR
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef TTime_HEADER
#define TTime_HEADER





/* necessary includes */
#include "define.hpp" // types/constants
//#define INLINE inline


/* global types and constants */

typedef TData TTime;
#define POS_IMPOSSIBLE_TIME ((TTime)1.0E10)
#define NEG_IMPOSSIBLE_TIME ((TTime)-1.0E10)
#define ZERO_TIME ((TTime)0.0)
#define ONE_SECOND ((TTime)1.0)
#define TEN_SECONDS ((TTime)10.0)
#define HUNDRED_SECONDS ((TTime)100.0)

/* data interface */


/* Time interface */
void set_zero_time();
TTime actual_time();
TTime passed_time_since(TTime);
char* decode_time(TTime);
TTime set_1970_time(const long,const long);
void get_1970_time(const TTime&,long&,long&);
void set_zero_time(const long);

/* inline implementation */






#endif /* TTime_HEADER */
