/*
//  Project       : 4D-Planner (4DP),
                    Intelligent Pilot Assistant (IPA)
//  Workpackage   : Basic Utility Files
//  
//  Author        : Frank Hoeppner (TH Emden)
                    with small changes of Hartmut Helmke (DLR)
//

  Description   : header of function module defbool
                  for documentation see defbool.tex
                  Source Documentation Handbook: tutorial-UTIL
  History       : see source file

//  Environment   : Source Documentation Handbook: tutorial-UTIL
//  Compiler      : gcc version egcs-2.91.66 19990314 (egcs-1.1.2 release) 
                    Compaq C++ V6.2-024 for Digital UNIX V4.0B  (Rev. 564) 
                    Microsoft Visual Studio Professional 5.1
                    IRIX Release 6.2  CC-Compiler Version ??
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

#ifndef defbool_HEADER
#define defbool_HEADER


/* necessary includes */
/* configuration include */
#ifdef HAVE_CONFIG_H
/*
//FILETREE_IFDEF HAVE_CONFIG_H
*/
#include "config.h"
/*
//FILETREE_ENDIF
*/
#endif
//#define INLINE inline

/* global types and constants */



/* DefBool interface */
inline bool itob(int i) { return (i ? true : false); }
bool stob(const char *);

/* inline implementation */


#endif /* defbool_HEADER */
