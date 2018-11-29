/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei der Funktionalität der Sequenzplanung und -bewertung
*/
#ifndef EvalSequence_HEADER
#define EvalSequence_HEADER

#include "GuestArray.h"
#include <algorithm> // wg. max
#include <fstream> //   ostream
#include <cmath> //  sqrt, ln, exp

typedef double (*T_EvalFunc)(int refTime, int targetTime, double expo);

double evaluateSequence(const GuestArray& guests,int bathEntryTime[]);
double evaluateSequence(GuestArray& guests,Guest vorgGast);
double evaluateSequenceFunc(GuestArray& guests,Guest vorgGast, T_EvalFunc func, double exp);

/**
   We return max(0, targetTime - refTime)
*/
inline double evaluateTime(int refTime,  int targetTime)
{
   return std::max(0.0, 
      (static_cast<double>(targetTime) - static_cast<double>(refTime))
      );
}

inline double evaluateTimeLinear(int refTime, int targetTime, double)
{
   double diff = static_cast<double>(targetTime) - static_cast<double>(refTime);
   return diff > 0? diff: 0;
/*   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? diff: 0;*/
/*   return std::max(0.0, 
      (static_cast<double>(targetTime) - static_cast<double>(refTime))
      );*/
/*   return std::max(0.0, 
      (static_cast<double>(targetTime - refTime))
      );*/
}


inline double evaluateTimeSquareInline
   (
   int refTime,
   int targetTime,
   double
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? diff*diff : 0;
}

double evaluateTimeSquare(int refTime,int targetTime, double);

inline double evaluateTimePow
   (
   int refTime,
   int targetTime,
   double expo
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? pow(diff,expo) : 0;
}

inline double evaluateTimeSquareRoot
   (
   int refTime,
   int targetTime,
   double
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? sqrt(diff) : 0;
}

double evaluateTimeExpo(int refTime,  int targetTime, double expo);

inline double evaluateTimeExpoInline
   (
   int refTime,
   int targetTime,
   double expo
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return (diff > 0)? exp(expo * log(diff)) : 0;
}

bool getBestSequence(const GuestArray& guests, GuestArray& bestSeq,
   double& bestseqValue);

bool getBestSequence(const GuestArray& guests, GuestArray& bestSeq,
  const Guest& vorgGast);
bool getBestSequenceFunc(const GuestArray& guests, GuestArray& bestSeq,
  const Guest& vorgGast, T_EvalFunc func, double expo);

void printSequenceTo(ostream& str,  const GuestArray& seq);


void makeGuestPermutation(const GuestArray& orgArr,
   GuestArray& permutGuests,  const int indexArr[]);

#endif /* EvalSequence_HEADER */
