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
\file zeitmess.h
\brief Utilities for runtime measurement

\author Prof. Dr. Hartmut Helmke (HHe)
*/


/**
\date 2010-05-17 HHe: new makros  ABS_ZEITMESS_SCHL_ID and  ZEITMESS_SCHL_ID
\date 2010-08-28 HHe: replaced char* by class string
*/




#ifndef __ZEITMESS
#define __ZEITMESS 1

#include <iostream>
#include <string>

#ifdef _MSC_VER
#   include <sys/timeb.h>
#   define SEK_TEIL 1000
#else
#   include <sys/time.h>
#   include <sys/time.h>
#   include <sys/resource.h>
#   define SEK_TEIL 1000000
#endif


/** Klasse zur Messung der Laufzeit des Programms, d.h. z.B. ohne die Zeit
   in der andere Prozesse rechnen.
  */
class K_TIMER
   {
     friend inline std::ostream &operator << (std::ostream &, const K_TIMER &);

   public:
   K_TIMER()
      { ges_zeit=0.0; anz_timer_aufrufe=0; max_time=0.0; anf_zeit=0; name="";
      };
   K_TIMER(char *);
   virtual ~ K_TIMER();

   void start_timer() { anf_zeit = konv_sys_zeit_milli_sec(); };
   double stop_timer()
      {
      double gest_zeit = konv_sys_zeit_milli_sec() - anf_zeit;
      anz_timer_aufrufe++;
      ges_zeit += gest_zeit;
      max_time = gest_zeit > max_time ? gest_zeit : max_time;
      return gest_zeit;
      };

   double durch_schn_zeit() const {return
      ((anz_timer_aufrufe  > 0) ? ges_zeit / anz_timer_aufrufe : 0);};

   double get_max_time() const {return max_time;}


   protected:
   double anf_zeit;   // Zeitpunkt zu dem die Zeitmessung begann, relativ zu
                      // einem beliebigen Anfangszeitpunkt.
   int anz_timer_aufrufe; // Anzahl der Messungen mit diesem Timer
   double ges_zeit;  // durchschnittliche Zeit
   double max_time;  // Maximale Zeit
   std::string name;      // wird bei der Zeitausgabe benutzt

   double konv_sys_zeit_milli_sec() const
      {
#    ifdef _MSC_VER
     struct timeb help;

     ftime(&help);
     return (  ((double) (help. time * SEK_TEIL + help. millitm))  / SEK_TEIL
            );
#    else
      struct rusage help_rusage;

      getrusage(RUSAGE_SELF, &help_rusage);
      return  (( (double) help_rusage. ru_utime. tv_sec * SEK_TEIL +
                   help_rusage. ru_utime. tv_usec) / SEK_TEIL );
#     endif
      };
   };


class K_R_TIMER:  // Klasse zur Realzeitmessung; auch mit nicht-aktiv Zeit
   public K_TIMER
   {
   public:
   K_R_TIMER(): K_TIMER() {;};
   K_R_TIMER(char * str): K_TIMER(str) {;};
   virtual ~ K_R_TIMER();

#  ifndef _MSC_VER
   void start_timer() { anf_zeit = konv_sys_zeit_milli_sec(); };
   double stop_timer()
      {
      double gest_zeit = konv_sys_zeit_milli_sec() - anf_zeit;
      anz_timer_aufrufe++;
      ges_zeit += gest_zeit;
      max_time = std::max(gest_zeit, max_time);
      return gest_zeit;
      };
      

   protected:
   double konv_sys_zeit_milli_sec() const
      {
      struct timeval helptime;

      gettimeofday(& helptime, 0);
      return  (( (double) helptime. tv_sec * SEK_TEIL +
                   helptime. tv_usec)            / SEK_TEIL );
      };
#  endif
   };


inline std::ostream &operator << (std::ostream & ostr, const K_TIMER & timer)
   {
   ostr << "\n";
   ostr << timer. name << ":";

   ostr << timer. anz_timer_aufrufe << " Messwerte, Durchschnitt: "
        << timer. durch_schn_zeit();
   ostr << " maximale Zeit: " << timer. max_time;
   return ostr;
   }

#  ifndef _MSC_VER
void Sleep (int delay);
#endif


#define ZEITMESS_SCHL(zeit_anz, zeit_str, zeit_file, zeit_akt)  \
  TYP_ZEITMESS_SCHL_ID(zeit_anz, zeit_str, zeit_file, K_TIMER, zeit_akt, ALWAYS_ID )
#define ABS_ZEITMESS_SCHL(zeit_anz, zeit_str, zeit_file, zeit_akt)  \
  TYP_ZEITMESS_SCHL_ID(zeit_anz, zeit_str, zeit_file, K_R_TIMER, zeit_akt, ALWAYS_ID )


#define TYP_ZEITMESS_SCHL_ID(zeit_anz, zeit_str, zeit_file, _type, zeit_akt, id) \
   {                                                                      \
   _type timer;                                                           \
   timer. start_timer();                                                  \
   for (int zeit_i = 0; zeit_i < zeit_anz; ++zeit_i)                      \
   {                                                                      \
      zeit_akt;                                                           \
   }                                                                      \
   double ges_zeit = timer. stop_timer();                                 \
   zeit_file << "\n" << zeit_str << " Durchschnittszeit: "                \
             << (ges_zeit / zeit_anz) << endl;                            \
   }


# endif // ZEITMESS
