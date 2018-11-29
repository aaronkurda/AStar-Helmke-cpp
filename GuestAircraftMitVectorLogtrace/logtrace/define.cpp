/*
//  Project       : Generic Trajectory Generator (GenTraG),
                    4D-Planner (4DP),
                    Intelligent Pilot Assistant (IPA)
//  Workpackage   : Basic Utility Files
//  
//  Author        : Dipl.-Inform Frank Hoeppner (OOW)
//

  Description   : implementation of function module define
                  for documentation see define.tex
                  Source Documentation Handbook: tutorial-UTIL
  History       :
  20010503 HHe history chunk added
  20010503 HHe: Extension deleted from C++ header files
  20010503 HHe: Chunk TBD Environment Author hinzugefuegt
  20012703 HHe: methods Constants::min and Constants::max renamed to
                  ... const_min and const_max, because Microsoft MFC
                  defines macros max and min
  20010111 HHe: many defines replaces bei const TYP.

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




/* necessary includes */
#include "define.hpp"


/* private functions */


/* data */
const double POS_IMPOSSIBLE=1.0E20;
const double NEG_IMPOSSIBLE=-1.0E20;
const double EPSILON=0.0001;
const double HALF=0.5;
const double ONE  (1.0);
const double TWO  (2.0);
const double DEKA (10.0);
const double HEKTO (100.0);
const double KILO (1000.0);
const int KILOBYTE (1024);
const double MEGA (1.0E6);
const int MEGABYTE (1048576);
const double GIGA (1.0E9);
const double TERA (1.0E12);

const double DEZI (0.1);
const double CENTI (0.01);
const double MILLI (0.001);
const double MICRO (1.0E-6);
const double NANO (1.0E-9);
const double PIKO (1.0E-12);
const double FEMTO (1.0E-15);
const float COLOR_ID_CONFLICTOUS[4]   = { 0.3f,0.0f,0.0f,1.0f };
const float COLOR_ID_UNVALIDATED[4]   = { 0.3f,0.3f,0.3f,1.0f };
const float COLOR_ID_NOCONFLICTYET[4] = { 0.7f,0.7f,0.7f,1.0f };
const float COLOR_ID_FINALCONFIG[4]   = { 0.0f,0.0f,1.0f,1.0 };
const float COLOR_ID_STARTCONFIG[4]   = { 0.0f,1.0f,0.0f,1.0f };
const float COLOR_ID_SELECTED[4]      = { 1.0f,1.0f,0.0f,1.0f };
const float COLOR_ID_CONSTRAINT[4]    = { 0.7f,0.0f,0.0f,1.0f };
const float COLOR_ID_MOVING[4]        = { 1.0f,0.0f,0.0f,1.0f };

/* implementation */


/* template instantiation */




