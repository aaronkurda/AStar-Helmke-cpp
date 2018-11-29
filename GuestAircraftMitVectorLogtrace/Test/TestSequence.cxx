/* 31-August-2010
   Copyright: H.Helmke, 
   �bungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Tests f�r die Sequenzplanung und -bewertung
*/
#include "TestSequence.h"
#include <iostream>   // cout
#include <algorithm>   // next_permutation

#include "EvalSequence.h"
#include "FileInput.h"
#include "zeitmess.h"
#include "BaseDir.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestSequence";

/**
Pr�fung der Funktion bool evaluateTime();

*/
bool testEvaluateTime()
{
   LOG_FUNCTION(CLASS_LT, "testEvaluateTime", "");
   return doublesEqual(evaluateTime(1500, 1800), 300) &&
         doublesEqual(evaluateTime(1800, 1500), 0);

}

/** Die 5 G�ste werden aus der Datei Gaeste.txt gelesen.
  F�r genau die dort in der Datei angegebene Reihenfolge 
  wird die Badbetretungszeit und  die Sequnenzbewertung ermittelt 
  und mit der erwarteten Sequenzbewertung (Summe der 
  Wartezeiten (hier 47 Minuten) verglichen.
*/
bool testEvaluateSequence()
{
   LOG_FUNCTION(CLASS_LT, "testEvaluateSequence", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste.txt");

   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);

   int* p_bathEntryTime = new int[guests.numberOfGuests()];
   double seqValue = evaluateSequence(guests, p_bathEntryTime);

   delete[] p_bathEntryTime;  // Heap wieder aufr�umen
   p_bathEntryTime = NULL;
   return (47==seqValue);
} // testEvaluateSequence


/** Die 5 G�ste werden aus der Datei Gaeste.txt gelesen.
   Hierf�r wird durch die Funktion getBestSequence die
   beste Sequenz ermittelt und mit den Erwartungen verglichen.
*/
bool testGetBestSequence5()
{
   LOG_FUNCTION(CLASS_LT, "testGetBestSequence5", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste.txt");

   GuestArray guests(3);
   readGuestsFromFile(inpFileName, guests);
   GuestArray bestSeq(3);

   double eval;
   bool retValue = getBestSequence(guests, bestSeq, eval);

   retValue =  (41.0 == eval) &&
      (bestSeq.getGuestName(0) == "Anna") &&
      (bestSeq.getGuestName(1) == "Mike") &&
      (bestSeq.getGuestName(2) == "Matthias") &&
      (bestSeq.getGuestName(3) == "Olga") &&
      (bestSeq.getGuestName(4) == "S�nke");
   return retValue;

} // testEvaluateSequence5



/** Die 3 G�ste werden aus der Datei GaesteTest2.txt gelesen.
   Hierf�r wird durch die Funktion getBestSequence die
   beste Sequenz ermittelt und mit den Erwartungen verglichen.
*/
bool testGetBestSequence()
{
   LOG_FUNCTION(CLASS_LT, "testGetBestSequence", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/GaesteTest2.txt");

   GuestArray guests(3);
   readGuestsFromFile(inpFileName, guests);
   GuestArray bestSeq(3);

   double eval;
   bool retValue = getBestSequence(guests, bestSeq, eval);
   retValue =  (32.0 == eval) &&
      (bestSeq.getGuestName(0) == "Matthias") &&
      (bestSeq.getGuestName(1) == "Olga") &&
      (bestSeq.getGuestName(2) == "S�nke");
   return retValue;

} // testEvaluateSequence


/** Es wird ein Array mit 4 Elementen erzeugt.
Hiervon gibt es 4! m�gliche Permutationen.
Diese werden eine nach der anderen durch 
next_permutation erzeugt. Dieser STL-Algorithmus
liefert zyklus jeweils die n�chste Permuation.
Der Test gibt immer true zur�ck.
*/
bool testPermuation()
{
   LOG_FUNCTION(CLASS_LT, "testPermuation", "");
   int arr[]={1, 3, 9, 2};
   int arrSize = sizeof(arr) / sizeof(int);
   int nFak = 1;  // Zur Speicherung von N Fakult�t
   for (int i = 0; i < arrSize; ++i)
   {
      nFak *= (i+1);
   }

   // alle nFak Permuationen erzeugen.
   for (int i = 0; i < nFak; ++i)
   {
     next_permutation(&arr[0], &(arr[arrSize]));
     TRACE((i+1) << ". Permutation: ");
     for (int j = 0; j < arrSize; ++j)
     {
        TRACE(arr[j] << " ");
     }
   }// for i
   return true;
}

/** Die 9 G�ste werden aus der Datei Gaeste9.txt gelesen.
   Hierf�r wird durch die Funktion getBestSequence die
   beste Sequenz ermittelt und mit den Erwartungen verglichen.
*/
bool testGetBestSequence9()
{
   LOG_FUNCTION(CLASS_LT, "testGetBestSequence9", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste9.txt");

   GuestArray guests(9);
   readGuestsFromFile(inpFileName, guests);
   GuestArray bestSeq(9);

   double eval; char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
      bool retValue = getBestSequence(guests, bestSeq, eval);
   double ges_zeit = timer. stop_timer();   // Timer stoppen
   cout << "Need time: " << ges_zeit << "\n";
   printSequenceTo(cout, bestSeq);

   retValue =  (123.0 == eval) &&
      (bestSeq.getGuestName(0) == "Anna") &&
      (bestSeq.getGuestName(1) == "Mike") &&
      (bestSeq.getGuestName(2) == "Joachim") &&
      (bestSeq.getGuestName(3) == "Matthias") &&
      (bestSeq.getGuestName(4) == "Hans") &&
      (bestSeq.getGuestName(5) == "Fritz") &&
      (bestSeq.getGuestName(6) == "Lothar") &&
      (bestSeq.getGuestName(7) == "Olga") &&
      (bestSeq.getGuestName(8) == "S�nke");

   return retValue;

} // testEvaluateSequence9
