/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Funktionalität der Sequenzplanung und -bewertung
*/
#include "EvalSequence.h"
#include "GuestArray.h"
#include "FileInput.h"
#include <algorithm> // function next_permutation
#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="EvaluateSeq";



double evaluateTimeSquare
   (
   int refTime,
   int targetTime,
   double
   )
{
   double diff = static_cast<double>(targetTime - refTime);
   return diff > 0? diff*diff : 0;
}



double evaluateTimeExpo
   (
   int refTime,
   int targetTime,
   double expo
   )
{
   double val=0.0;
   for (int i=0; i< 1000;++i){
      val += i * sin(3.14/(i+1));
   }

   double diff = static_cast<double>(targetTime - refTime);
   return (diff > 0)? exp(expo * log(diff)) : 0;
}


/**
Der Gast steht um guests[i]wakeUpTime auf
und benötigt im Bad guests[i].bathUsageTime Minuten.
Es wird davon ausgegangen, dass das Bad nacheinander
in der Reihenfolge guests[0], guests[1], ... unter Beachtung von
bathUsageTime und frühester Zeit (wakeUpTime) das Bad betreten.
Dann wird ermittelt, zu welcher Zeit sie 
bei gegebener Sequenz dann das Bad betreten.
Der Rückgabewert der Funktion ist die Bewertung der Sequenz
als Summe der Wartezeiten.

\param[in]   guests (Array)
\param[out]  bathEntryTime (Array)
\return Sequenzbewertung
*/
double evaluateSequence
   (
   const GuestArray& guests,
   int bathEntryTime[]
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "evaluateSequence", "");
   if (guests.numberOfGuests() < 1)
   {
      return -1.0;
   }
   bathEntryTime[0] = guests.getWakeUpTime(0);

   double seqValue = 0.0;

   for (int i=1; i < guests.numberOfGuests(); ++i)
   {
      // Maximum aus Aufstehzeit und Verlassen durch Vorgänger
      bathEntryTime[i] = max(guests.getWakeUpTime(i),  
         bathEntryTime[i-1] + guests.getBathUsageTime(i-1));

      seqValue += evaluateTime(guests.getWakeUpTime(i), bathEntryTime[i]);
   }// for i

   return seqValue;

}// evaluateSequence



/**
Der Gast steht um guests[i]wakeUpTime auf
und benötigt im Bad guests[i].bathUsageTime Minuten.
Es wird davon ausgegangen, dass das Bad nacheinander
in der Reihenfolge guests[0], guests[1], ... unter Beachtung von
bathUsageTime und frühester Zeit (wakeUpTime) das Bad betreten.
Dann wird ermittelt, zu welcher Zeit sie 
bei gegebener Sequenz dann das Bad betreten.
Der Rückgabewert der Funktion ist die Bewertung der Sequenz
als Summe der Wartezeiten.

\param       guests (Array)
\param[in]   vorgGast
\return Sequenzbewertung
*/
double evaluateSequence
   (
   GuestArray& guests,
   Guest vorgGast
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "evaluateSequence", "");

   if (guests.numberOfGuests() < 1)
   {
      return -1.0;
   }
   int bathEntryTime = max(guests.getWakeUpTime(0),  
      vorgGast.getBathEntryTime() + vorgGast.getBathUsageTime());

   guests.setBathEntryTime(0, bathEntryTime);
   double seqValue = evaluateTime(guests.getWakeUpTime(0), bathEntryTime);

   for (int i=1; i < guests.numberOfGuests(); ++i)
   {
      // Maximum aus Aufstehzeit und Verlassen durch Vorgänger
      bathEntryTime = max(guests.getWakeUpTime(i),  
         guests.getBathEntryTime(i-1) + guests.getBathUsageTime(i-1));
      guests.setBathEntryTime(i, bathEntryTime);
      seqValue += evaluateTime(guests.getWakeUpTime(i), bathEntryTime);
   }// for i

   return seqValue;

}// evaluateSequence


/**
Der Gast steht um guests[i]wakeUpTime auf
und benötigt im Bad guests[i].bathUsageTime Minuten.
Es wird davon ausgegangen, dass das Bad nacheinander
in der Reihenfolge guests[0], guests[1], ... unter Beachtung von
bathUsageTime und frühester Zeit (wakeUpTime) das Bad betreten.
Dann wird ermittelt, zu welcher Zeit sie 
bei gegebener Sequenz dann das Bad betreten.
Der Rückgabewert der Funktion ist die Bewertung der Sequenz
als Summe der Wartezeiten.

\param       guests (Array)
\param[in]   vorgGast
\param[in]   func, evaluation function
\param[in]   expo, parameter maybe used by func
\return Sequenzbewertung
*/
double evaluateSequenceFunc
   (
   GuestArray& guests,
   Guest vorgGast,
   T_EvalFunc func,
   double expo
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "evaluateSequenceFunc", "");
   if (guests.numberOfGuests() < 1)
   {
      return -1.0;
   }

   int bathEntryTime = max(guests.getWakeUpTime(0),  
      vorgGast.getBathEntryTime() + vorgGast.getBathUsageTime());

   guests.setBathEntryTime(0, bathEntryTime);
   double seqValue = func(guests.getWakeUpTime(0), bathEntryTime, expo);

   for (int i=1; i < guests.numberOfGuests(); ++i)
   {
      // Maximum aus Aufstehzeit und Verlassen durch Vorgänger
      bathEntryTime = max(guests.getWakeUpTime(i),  
         guests.getBathEntryTime(i-1) + guests.getBathUsageTime(i-1));
      guests.setBathEntryTime(i, bathEntryTime);
      seqValue += func(guests.getWakeUpTime(i), bathEntryTime,expo);
   }// for i

   return seqValue;

}// evaluateSequenceFunc



/**  Die Funktion erstellt eine Permuation von orgArr,
     wie sie im Array indexArr beschrieben ist, d.h.
     das Element i aus dem Array orgArr wird an die Stelle
     indexArr[i] im Array permutGuests kopiert.
\param[in]    orgArr
\param[out]   permutGuests
\param[in]    indexArr
*/
void makeGuestPermutation
   (
   const GuestArray& orgArr,
   GuestArray& permutGuests,
   const int indexArr[]
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "makeGuestPermutation", "");
   for (int i=0; i < orgArr.numberOfGuests(); ++i)
   {      
      permutGuests.setGuest(indexArr[i], orgArr.getGuest(i) );
   }
} // makeGuestPermutation


/**
Die Funktion ermittelt die optimale Reihenfolge für die Gäste
im Array guests für die Minimierung der Wartezeiten
bei der Badbelegung.

\param[in]   guests  (Array)
\param[out]  bestSeq (Array)
\param[out]  bestseqValue Bewertung für beste Sequenz
\return      true, wenn erfolgreich
*/
bool getBestSequence
   (
   const GuestArray& guests,
   GuestArray& bestSeq,
   double& bestseqValue
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequence", "");
   if (guests.numberOfGuests() < 1)
   {
      return false;
   }

   GuestArray permutGuests(guests.numberOfGuests() );
   permutGuests = guests;
   int* p_bathEntryTimes = new int[guests.numberOfGuests()];

   // Array mit den Zahlen 0, 1,... (Anzahl Gäste -1) erstellen
   int* indexArr = new int[guests.numberOfGuests()];
   int nFak=1;
   for (int i= 0; i < guests.numberOfGuests(); ++i)
   {
      indexArr[i] = i;
      nFak *= (i+1);
   }

   bestseqValue = 100000000.0; // ganz schlechter Wert
   for (int i= 0; i < nFak; ++i)
   {
      makeGuestPermutation(guests, permutGuests, indexArr);
      double seqValue = evaluateSequence(permutGuests, p_bathEntryTimes);
      if (seqValue < bestseqValue)
      {
         bestSeq=permutGuests;
         bestseqValue = seqValue;
      }
     next_permutation(&indexArr[0], &(indexArr[guests.numberOfGuests()] ));
   }// for i

      delete[] indexArr;
      delete[] p_bathEntryTimes;
      return true;

}// getBestSequence


/**
Die Funktion ermittelt die optimale Reihenfolge für die Gäste
im Array guests für die Minimierung der Wartezeiten
bei der Badbelegung. Für den ersten Gast in guests ist
vorgGast der Vorgänger

\param[in]   guests  (Array)
\param[out]  bestSeq (Array)
\param[in]   vorgGast
\return      true, wenn erfolgreich
*/
bool getBestSequence
   (
   const GuestArray& guests,
   GuestArray& bestSeq,
   const Guest& vorgGast
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequence", "");
   GuestArray permutGuests(guests.numberOfGuests() );
   permutGuests=guests;

   // Array mit den Zahlen 0, 1,... (Anzahl Gäste -1) erstellen
   int* indexArr = new int[guests.numberOfGuests()];
   int nFak=1;
   for (int i= 0; i < guests.numberOfGuests(); ++i)
   {
      indexArr[i] = i;
      nFak *= (i+1);
   }

   double bestseqValue = 100000000.0; // ganz schlechter Wert
   for (int i= 0; i < nFak; ++i)
   {
      makeGuestPermutation(guests, permutGuests, indexArr);
      double seqValue = evaluateSequence(permutGuests, vorgGast);
      if (seqValue < bestseqValue)
      {
         bestSeq=permutGuests;
         bestseqValue = seqValue;
      }
     next_permutation(&indexArr[0], &(indexArr[guests.numberOfGuests()] ));
   }// for i

      delete[] indexArr;
      return true;

}// getBestSequence



/**
Die Funktion ermittelt die optimale Reihenfolge für die Gäste
im Array guests für die Minimierung der Wartezeiten
bei der Badbelegung. Für den ersten Gast in guests ist
vorgGast der Vorgänger
Bewertungsfunktion func und expo sind Parameter.

\param[in]   guests  (Array)
\param[out]  bestSeq (Array)
\param[in]   vorgGast
\param[in]   func, evaluation function
\param]in]   expo, exponent maybe used by func
\return      true, wenn erfolgreich
*/
bool getBestSequenceFunc
   (
   const GuestArray& guests,
   GuestArray& bestSeq,
   const Guest& vorgGast,
   T_EvalFunc func,
   double expo
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "getBestSequenceFunc", "");
   GuestArray permutGuests(guests.numberOfGuests() );
   permutGuests=guests;

   // Array mit den Zahlen 0, 1,... (Anzahl Gäste -1) erstellen
   int* indexArr = new int[guests.numberOfGuests()];
   int nFak=1;
   for (int i= 0; i < guests.numberOfGuests(); ++i)
   {
      indexArr[i] = i;
      nFak *= (i+1);
   }

   double bestseqValue = 100000000.0; // ganz schlechter Wert
   for (int i= 0; i < nFak; ++i)
   {
      makeGuestPermutation(guests, permutGuests, indexArr);
      double seqValue = evaluateSequenceFunc(permutGuests, vorgGast, func, expo);
      if (seqValue < bestseqValue)
      {
         bestSeq=permutGuests;
         bestseqValue = seqValue;
      }
     next_permutation(&indexArr[0], &(indexArr[guests.numberOfGuests()] ));
   }// for i

      delete[] indexArr;
      return true;

}// getBestSequenceFunc



/** Die Sequenz (Gäste-Array) seq wird bewertet, d.h.
die Badbelegungszeiten und die Wartezeit ermittelt.
Das Array und die Badzeiten werden zusammen mit den
Gesamtwartenzeiten ausgegeben.
*/
void printSequenceTo
   (
   ostream& str,
   const GuestArray& seq
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "printSequenceTo", "");
   int* p_bathEntryTimes = new int[seq.numberOfGuests()];
   double seqValue = evaluateSequence(seq, p_bathEntryTimes);

   const int columnSize=12;
   for (int i=0; i < seq.numberOfGuests(); ++i)
   {
      str.width(columnSize);
      str << seq.getGuestName(i);
      str.width(6);
      printTimeToFile(seq.getWakeUpTime(i), str);
      str.width(5);
      str << seq.getBathUsageTime(i);
      str << "   ";
      printTimeToFile(p_bathEntryTimes[i], str);
      str << "\n";
   }
   str << "Gesamtwartezeit : " << seqValue << " s\n";

   delete[] p_bathEntryTimes;
}
