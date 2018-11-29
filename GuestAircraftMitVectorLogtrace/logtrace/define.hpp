/*
//  Project       : Generic Trajectory Generator (GenTraG),
                    4D-Planner (4DP),
                    Intelligent Pilot Assistant (IPA)
//  Workpackage   : Basic Utility Files
//  
//  Author        : Dipl.-Inform Frank Hoeppner (OOW)
//

  Description   : header of function module define
                  for documentation see define.tex
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
//                                2000-2006 DLR
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

#ifndef Define_HEADER
#define Define_HEADER





/* necessary includes */
#include <climits>
#include <cfloat> // defines FLT_MAX
#include <math.h>
#include "defbool.hpp"
//#define INLINE inline


/* global types and constants */

typedef double TData;
typedef float TCost;
typedef float TRange;
typedef float TLambda;
typedef unsigned int word;
typedef char * const char_cp;
typedef char const * char_cc; // const char * == char const *
typedef const char * const char_cpc;
#define SQRT2 (1.414213562373)
#define SQRT3 (1.7320508076)
#define SQRT5 (2.2360679775)
#define ONE_NAUTICAL_MILE (1852.0)
#define ONE_FEET (0.3048)
#define GRAVITATIONAL_ACCELERATION (9.8066)
#define UNIVERSAL_GAS_CONSTANT_AIR (287.04)
#define ISENTROPIC_EXPANSION_AIR (1.4)

/* data interface */
extern const double POS_IMPOSSIBL;
extern const double NEG_IMPOSSIBLE;
extern const double EPSILON;
extern const double HALF;
extern const double ONE;
extern const double TWO;
extern const double DEKA;
extern const double HEKTO;
extern const double KILO;
extern const int KILOBYTE;
extern const double MEGA;
extern const int MEGABYTE;
extern const double GIGA;
extern const double TERA;

extern const double DEZI;
extern const double CENTI;
extern const double MILLI;
extern const double MICRO;
extern const double NANO;
extern const double PIKO;
extern const double FEMTO;
#define MIN_PER_HOUR (60)
#define SEC_PER_MIN (60)
#define FEET_TO_METRE (ONE_FEET)
#define METRE_TO_FEET (1.0/FEET_TO_METRE)
#define KTS_TO_MPS (ONE_NAUTICAL_MILE/(SEC_PER_MIN*MIN_PER_HOUR)) 
#define MPS_TO_KTS (1.0/KTS_TO_MPS) 
#define NM_TO_M (ONE_NAUTICAL_MILE)
#define M_TO_NM (1.0/ONE_NAUTICAL_MILE)
#define MIN_LAMBDA ((TLambda)0.0)
#define MAX_LAMBDA ((TLambda)1.0)
#define POS_IMPOSSIBLE_LAMBDA ((TLambda)2.0)
#define NEG_IMPOSSIBLE_LAMBDA ((TLambda)-2.0)
#define NO_CONFLICT_LAMBDA POS_IMPOSSIBLE_LAMBDA
#define CONFLICT_LAMBDA ((TLambda)0.0)
#define ZERO_COST (0.0)
#define POS_IMPOSSIBLE_COST (1.0E20)
#define NEG_IMPOSSIBLE_COST (-1.0E20)
#define ZERO_RANGE (0.0)
#define EPSILON_RANGE (0.001)
#define POS_IMPOSSIBLE_RANGE (1.0E20)
#define NEG_IMPOSSIBLE_RANGE (-1.0E20)
#define BEEP_CHAR char(7)
#define ESCAPE_ASCII 27
#define BLANK_ASCII 32
#define PLUS_ASCII 43
#define COLON_ASCII 44
#define MINUS_ASCII 45
#define POINT_ASCII 46
#define LOWER_A_ASCII 97
#define LOWER_B_ASCII 98
#define LOWER_C_ASCII 99
#define LOWER_D_ASCII 100
#define LOWER_E_ASCII 101
#define LOWER_F_ASCII 102
#define LOWER_G_ASCII 103
#define LOWER_H_ASCII 104
#define LOWER_I_ASCII 105
#define LOWER_J_ASCII 106
#define LOWER_K_ASCII 107
#define LOWER_L_ASCII 108
#define LOWER_M_ASCII 109
#define LOWER_N_ASCII 110
#define LOWER_O_ASCII 111
#define LOWER_P_ASCII 112
#define LOWER_Q_ASCII 113
#define LOWER_R_ASCII 114
#define LOWER_S_ASCII 115
#define LOWER_T_ASCII 116
#define LOWER_U_ASCII 117
#define LOWER_V_ASCII 118
#define LOWER_W_ASCII 119
#define LOWER_X_ASCII 120
#define LOWER_Y_ASCII 121
#define LOWER_Z_ASCII 122
template <class DATA>
struct Constants
  {
  inline DATA const_max() const;
  inline DATA const_min() const;
  };
#define STATE_ClosedNode (1)
#define STATE_ExpandedNode (2)
#define STATE_Duplicate (4)
#define STATE_Upgraded (8)
#define STATE_Downgraded (16)
#define ACTION_SuccToBeDeleted (32)
#define ACTION_ToBeDeleted (64)
#define ACTION_ToBeExpanded (128)
#define ACTION_ToBeOpened (256)
#define ACTION_ToBeClosed (512)

#define MAX_LEN_TAG_STRING 15
#define MAX_LEN_NAME_STRING 31
//#define MAX_LEN_BUFFER_STRING 127

#define REDRAW_STARTFINAL ((int)1)
#define REDRAW_CONSTRAINTS ((int)2)
#define REDRAW_STATE ((int)4)
#define REDRAW_SELECTION ((int)8)
#define REDRAW_TIME_DEPENDENT ((int)16)

extern const float COLOR_ID_CONFLICTOUS[];
extern const float COLOR_ID_UNVALIDATED[];
extern const float COLOR_ID_NOCONFLICTYET[];
extern const float COLOR_ID_FINALCONFIG[];
extern const float COLOR_ID_STARTCONFIG[];
extern const float COLOR_ID_SELECTED[];
extern const float COLOR_ID_CONSTRAINT[];
extern const float COLOR_ID_MOVING[];

/* Constants interface */
#define SQR(a)      ((a)*(a))
#define ZERO(a)     (fabs(a)<EPSILON)
#define NOTZERO(a)  (fabs(a)>=EPSILON)
#define POSITIVE(a) ((a) >  EPSILON)
#define NEGATIVE(a) ((a) < -EPSILON)
#define EQUAL ==
#define NOT(expr) (!(expr))
#define SET_BIT(i) (1<<i)
#define IS_MASKED(status,mask,tag) ( ((status) & (mask)) == tag )
#define IS_TAG(status,tag) ( ((status) & (tag)) != 0 )
#define IS_NO_TAG(status,tag) ( ((status) & (tag)) == 0 )
#define SET_TAG(status,tag) (status) |= (tag)
#define CLEAR_TAG(status,tag) (status) &= (~(tag))




#endif /* Define_HEADER */
