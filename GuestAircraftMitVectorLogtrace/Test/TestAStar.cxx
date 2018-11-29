/* 18-November-2017
Copyright: H.Helmke,
Quellcode der Tests fuer den A*-Algorithmus
*/

#include "TestAStar.h"
#include "TestSequence.h"
#include <iostream>   // cout

#include "AStar.h"
#include "FileInput.h"
#include "EvalSequence.h" // printSequenceTo
#include "BaseDir.h" // wg. FILE_BASE_DIR_NAME 
#include "zeitmess.h"

#include "dosFarben.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestAStar";


/** Die 5 Gaeste werden aus der Datei Gaeste.txt gelesen.
Hierfuer wird durch die Funktion getBestSequenceAStar die
beste Sequenz mit dem A*-Algorithmus ermittelt
und mit den Erwartungen verglichen.
*/
bool testOptiWithAStar() {
   LOG_FUNCTION(CLASS_LT, "testOptiWithAStar", "");
   GuestArray guests(3);  // wird in readGuestsFromFile vergroessert
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste.txt");
   readGuestsFromFile(inpFileName, guests);

   GuestArray bestSeq(3);
   AStar astarInst;
   bool retValue = astarInst.getBestSequence(guests, bestSeq);

   retValue = (41.0 == astarInst.getEvalForBestSeq()) &&
      (bestSeq.getGuestName(0) == "Anna") &&
      (bestSeq.getGuestName(1) == "Mike") &&
      (bestSeq.getGuestName(2) == "Matthias") &&
      (bestSeq.getGuestName(3) == "Olga") &&
      (bestSeq.getGuestName(4) == "Sönke") && retValue;
   return retValue;
}



bool testSeqValueAndSequence
(
string inpFileName,
const vector<string>& ar_expectedSeq,
double ad_expValue
)
/* Die Gaeste werden aus derDatei inpFileName eingelesen.  
   Diese Gaestesequenz wird mit dem A*-Algorithmus optimiert.
   Die Funktion liefert false, wenn eine der folgenden
   Bedingungen eintritt
   - Einlesen aus der Datei scheitert.
   - Optimierung durch Methode getBestSequence von A* scheitert
   - Die ermittelte Wartezeit fuer die beste Bewertung ist
     verschieden von ad_expValue
   -  Die Reihenfolge der Gaestenamen in ar_expectedSeq
      ist verschieden von der Reihenfolge der Gaeste
      der als optimal ermittelten Sequenz (Test entfaellt,
      wenn ar_expectedSeq leer ist)
   -  Die Zeit, die die getBestSequence benoetigt, ist 
      groesser als 60 Sekunden.

   ad_expValue gibt den erwarteten Wert an.
   Ist der Wert negativ, wird der positive Wert genommen, allerdings
   ist der Test auch dann erfolgreich, wenn das hier gelieferte
   Ergebnisse besser als ad_expValue ist.
   In diesem Fall ist somit das beste Ergebnis fuer ar_expectedSeq
   bisher nicht bekannt, sondern lediglich eine untere Schranke dafuer.
   */
{
   LOG_FUNCTION(CLASS_LT, "testSeqValueAndSequence", "");

   GuestArray guests(3);  // wird in readGuestsFromFile vergroessert
   readGuestsFromFile(inpFileName, guests);

   if (ar_expectedSeq.size() == 0)
   {
      if (guests.size() < 3)
      {
         WARNING("number of guests (" << guests.size() << ") read from " << inpFileName
                 << " is 0 ");
         return false;
      }
   }
   else
   {
      if (guests.size() != ar_expectedSeq.size())
      {
         WARNING("number of guests (" << guests.size() << ") read from " << inpFileName
                 << " different from expected one " << ar_expectedSeq.size());
         return false;
      }
   }
   GuestArray bestSeq(3);  // wird ggf. vergroessert
   AStar astarInst;
   K_R_TIMER timer;
   timer.start_timer();
   bool retValue = astarInst.getBestSequence(guests, bestSeq);
   double ges_zeit = timer.stop_timer(); 
   cout << "Needed time for " << bestSeq.size() << " elements: " 
      << ges_zeit << ", waiting time " << astarInst.getEvalForBestSeq() << endl;

   if (ad_expValue > 0)
   {
      retValue = (fabs(ad_expValue - astarInst.getEvalForBestSeq()) < 0.1) && retValue;
   }
   else
   {
      // also better values are allowed
      ad_expValue = -ad_expValue;
      if (ad_expValue > astarInst.getEvalForBestSeq() + 0.1)
      {
         printScreenColorOnceVal(cout, GREEN_SCREEN_COLOR, "Result better than expected\n");
      }
      else
      {
         retValue = (fabs(ad_expValue - astarInst.getEvalForBestSeq()) < 0.1) && retValue;
      }
   }

   retValue = (ar_expectedSeq.size() == 0 || 
               bestSeq.size() == ar_expectedSeq.size()) && retValue;
   int i = 0;
   for (auto iter : ar_expectedSeq)
   {
      retValue = retValue && bestSeq.getGuestName(i) == iter;
      ++i;
   }

   if (retValue == false)
   {
      printSequenceTo(cout, bestSeq);
      cout << "expected Value is: " << ad_expValue
         << ", we got " << astarInst.getEvalForBestSeq() << endl;
   }

   retValue = retValue && ges_zeit <= 60.0;

   return retValue;

   } // bool testSeqValueAndSequence


#ifndef WETTBEWERB_GUESTS


/**
Dieser Test implementiert das Abnahmekritierium:
Sie bekommen Gästedateien mit 5-9 Gästen.
Alle Ergebnisse richtig innerhalb von 60 Sekunden Rechenzeit

*/
bool test5to9SimpleGuestFiles() {
   LOG_FUNCTION(CLASS_LT, "test5to9SimpleGuestFiles", "");
   bool retValue = true;

   retValue = retValue && testSeqValueAndSequence(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbGuests2018/Gaeste5Linear.txt",
      { "Anna", "Mike", "Matthias", "Olga", "Sönke" }, 41.0);


   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   cout << endl << endl;
   return retValue;

} // test5to9SimpleGuestFiles


/**
Dieser Test implementiert das Abnahmekritierium:
Sie bekommen einfache Dateien mit 10 bis 15 Gästen:
Alle Ergebnisse richtig innerhalb von 60 Sekunden Rechenzeit
*/
bool test10to15SimpleGuestFiles() {
   LOG_FUNCTION(CLASS_LT, "test10to15SimpleGuestFiles", "");
   bool retValue = true;

   // Die Dateien werden noch veraendert und damit auch die erwarteten Ergebnisse
   retValue = retValue && testSeqValueAndSequence(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbGuests2018/Gaeste5Linear.txt",
      { "Anna", "Mike", "Matthias", "Olga", "Sönke" }, 41.0);

   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   cout << endl << endl;
   return retValue;

} // test10to15SimpleGuestFiles


/**
Dieser Test implementiert das Abnahmekritierium:
Sie bekommen Dateien mit 5 bis 15 Gästen:
Welche Größe schaffen Sie noch in 60 Sekunden Rechenzeit:
Platz 1, 2, 3 (wenn weniger als 3 Gruppen, bin ich auch eine Gruppe

*/
bool test5to15DifficultGuestFiles() {
   LOG_FUNCTION(CLASS_LT, "test5to15DifficultGuestFiles", "");
   bool retValue = true;

   retValue = retValue && testSeqValueAndSequence(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbGuests2018/Gaeste5Linear.txt",
      { "Anna", "Mike", "Matthias", "Olga", "Sönke" }, 41.0);


   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   cout << endl << endl;
   return retValue;

} // test5to15DifficultGuestFiles


#endif