/*
//  Project       : Generic Trajectory Generator (GenTraG),
                    4D-Planner (4DP),
                    Intelligent Pilot Assistant (IPA)
//  Workpackage   : Basic Utility Files
//  
//  Author        : Dipl.-Inform Frank Hoeppner (OOW)
//

  Description   : implementation of function module ttime
                  for documentation see ttime.tex
                  Source Documentation Handbook: tutorial-UTIL
  History       :
  20010503 HHe: Chunk TBD Environment Author hinzugefuegt
  20031110 LC: inlcude fstream statt fstream.h

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
/* configuration include */
#include "ttime.hpp"
#if defined(__BORLANDC__) || defined(_MSC_VER)
#  include <sys\timeb.h>
#  include <time.h>
#  include <math.h>
// struct timeb
//   {
//   long  time;
//   short millitm;
//   short timezone;
//   short dstflag;
//   };
#endif
#ifdef __unix
#  include <sys/time.h>
#  include <math.h>
#  include <fstream>
// struct timeval
//   {
//   long tv_sec;        /* seconds since Jan. 1, 1970 */
//   long tv_usec;       /* and microseconds */
//   };
#endif


/* private functions */
#if defined(__BORLANDC__) || defined(_MSC_VER)
  struct timeb init_zero_time(const long);
#elif __unix
  struct timeval init_zero_time(const long);
#else
# error "No system specific code provided."
#endif

/* data */
#if defined(__BORLANDC__) || defined(_MSC_VER)
  struct timeb g_zerotime = init_zero_time(0);
#elif __unix
  struct timeval g_zerotime = init_zero_time(0);
#else
# error "No system specific code provided."
#endif

/* implementation */

void 
set_zero_time
  (
  ) 
  { 
  g_zerotime = init_zero_time(0);
  }

TTime actual_time
  (
  )
  {
  TTime floattime;
  #if defined(__BORLANDC__) || defined(_MSC_VER)
    struct timeb systemtime;
    ftime(&systemtime); 
    floattime =
                (systemtime.time-g_zerotime.time)
      + MILLI * (systemtime.millitm-g_zerotime.millitm);
  #elif __unix
    struct timeval systemtime;
    struct timezone systemzone;
    gettimeofday(&systemtime,&systemzone);
    floattime = 
                (systemtime.tv_sec-g_zerotime.tv_sec) 
      + MICRO * (systemtime.tv_usec);
  #else
  # error "No system specific code provided."
  #endif
  return floattime;
  }

TTime passed_time_since
  (
  TTime a_floattime
  )
  {
  TTime now( actual_time() );
  return (now - a_floattime);
  }

char* decode_time
  (
  TTime a_floattime
  )
  {
  #if defined(__BORLANDC__) || defined(_MSC_VER)
    long l = (long int)(a_floattime) + (long int) g_zerotime.time; 
    return asctime( gmtime( (const time_t*) &l ) );
  #elif __unix
    time_t l = (time_t)(a_floattime) + (time_t)(g_zerotime.tv_sec);
    return asctime( gmtime( &l ) );
  #else
  # error "No system specific code provided."
  #endif
  }

void set_zero_time
  (
  const long a_sec
  )
  {
  g_zerotime = init_zero_time(a_sec);
  }

TTime set_1970_time
  (
  const long a_sec,
  const long a_usec
  )
  {
  TTime floattime;
  #if defined(__BORLANDC__) || defined(_MSC_VER)
    floattime = 
                (a_sec-g_zerotime.time)
      + MICRO * (a_usec-g_zerotime.millitm*KILO);
  #elif __unix
    floattime = 
                 (a_sec-g_zerotime.tv_sec) 
       + MICRO * a_usec; 
  #else
  # error "No system specific code provided."
  #endif
  return floattime;
  }

void get_1970_time
  (
  const TTime& a_floattime,
  long &ar_sec,
  long &ar_usec
  )
  {
  #if defined(__BORLANDC__) || defined(_MSC_VER)
    long secs = (long)(a_floattime);
    ar_sec  = (long) g_zerotime.time + secs; 
    ar_usec = (long)(g_zerotime.millitm+MEGA*(a_floattime-secs)); 
    if ((secs<0) && (ar_sec>0) && (ar_usec<0))
      {
      --ar_sec;
      ar_usec += (long)MEGA;
      }
  #elif __unix
    long secs = (long)(a_floattime); 
    ar_sec  = g_zerotime.tv_sec+secs; 
    ar_usec = (long)(MEGA*(a_floattime-secs)); 
    if ((secs<0) && (ar_sec>0) && (ar_usec<0))
      {
      --ar_sec;
      ar_usec += (long)MEGA;
      }
  #else
  # error "No system specific code provided."
  #endif
  }
#if defined(__BORLANDC__) || defined(_MSC_VER)
  struct timeb init_zero_time
    (
    const long a_sec
    )
    {
    struct timeb systemtime;
    if (a_sec==0)
      {
      ftime(&systemtime);
      }
    else
      {
      systemtime.time = a_sec;
      systemtime.millitm = 0;
      }
    return systemtime;
    }
#elif __unix
struct timeval
init_zero_time
  (
  const long a_sec
  )
  {
  struct timeval systemtime;
  struct timezone systemzone;
  if (a_sec==0)
    {
    gettimeofday(&systemtime,&systemzone);
    }
  else
    {
    systemtime.tv_sec = a_sec;
    }
  systemtime.tv_usec=0;
  return systemtime;
  }
#else
# error "No system specific code provided."
#endif

/* template instantiation */




