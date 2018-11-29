/* 18-November-2017
Copyright: H.Helmke,
Quellcode der Tests fuer den A*-Algorithmus
*/

#include "TestAStarAcft.h"
#include "TestSequence.h"
#include "dosFarben.h"
#include <iostream>   // cout
#include <vector>

#include "AStarAcft.h"
#include "QualityFuncAircraft.h"
#include "Aircraft.h"
#include "FileInput.h"
#include "BaseDir.h" // wg. FILE_BASE_DIR_NAME 
#include "zeitmess.h"
#include "testMakros.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestAStarAcft";


/** Die 5 Acft werden aus der Datei Aircraft5SameQuali.txt gelesen.
Hierfuer wird durch die Funktion getBestSequenceAStar die
beste Sequenz mit dem A*-Algorithmus ermittelt 
und mit den Erwartungen verglichen.
Es werden nur die fruehesten Zeiten beachtet.
*/
bool testOptiWithAStarAcft() {
   LOG_FUNCTION(CLASS_LT, "testOptiWithAStarAcft", "");

   vector<Aircraft> seq;
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft5SameQuali.txt");
   readElemsFromFile(inpFileName, seq);

   vector<Aircraft> bestSeq;
   QualityFuncAircraft qualiFuncOnlyEarly;
   qualiFuncOnlyEarly.SetCriteriaWeights(1.0, 1.0, 1.0);
   AStarAcft astarInst(qualiFuncOnlyEarly);

   bool retValue = astarInst.getBestSequence(seq, bestSeq);

   double expectedValue = 11250.0;
   retValue = retValue && 
      (DoubleEqual(expectedValue, astarInst.getEvalForBestSeq(), 0.01) &&
      (bestSeq.at(0).getCallsign() == "BAW766") &&
      (bestSeq.at(1).getCallsign() == "AF8797") &&
      (bestSeq.at(2).getCallsign() == "IBE765") &&
      (bestSeq.at(3).getCallsign() == "AF458") &&
      (bestSeq.at(4).getCallsign() == "AF345"));

   if (retValue == false)
   {
      printSequenceTo(cout, bestSeq, astarInst.getEvalForBestSeq());
      cout << "expected Value is: " << expectedValue
         << ", we got " << astarInst.getEvalForBestSeq() << endl;
   }

   return retValue;

} // testOptiWithAStarAcft()




/** Die Sequenz (Gäste-Array) seq wird bewertet, d.h.
die Badbelegungszeiten und die Wartezeit ermittelt.
Das Array und die Badzeiten werden zusammen mit den
Gesamtwartenzeiten ausgegeben.
*/
void printSequenceTo
(
   ostream& str,
   const vector<Aircraft>& seq,
   double eval
)
{
   LOG_FUNCTION(CLASS_LT, "printSequenceTo", "");

   for (int i = 0; i < static_cast<int>(seq.size()); ++i)
   {
      seq[i].write(str);
      str << "\n";
   }
   str << "Gesamtbewertung : " << eval << " s\n";
}



bool testSeqValueAndSequenceAcft
(
   string inpFileName,
   const vector<string>& ar_expectedSeq,
   double ad_expValue,
   const QualityFuncAircraft& ar_qualiFunc
)
/* Die Flugzeuge werden aus derDatei inpFileName eingelesen.
   Diese Flugzeugsequenz wird mit dem A*-Algorithmus optimiert.
   Die Funktion liefert false, wenn eine der folgenden
   Bedingungen eintritt
   - Einlesen aus der Datei scheitert.
   - Optimierung durch Methode getBestSequence von A* scheitert
   - Die ermittelte Wartezeit fuer die beste Bewertung ist
     verschieden von ad_expValue
   -  Die Reihenfolge der Flugzeugnamen in ar_expectedSeq
      ist verschieden von der Reihenfolge der Flugzeuge
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
   LOG_FUNCTION(CLASS_LT, "testSeqValueAndSequenceAcft", "");

   Aircraft vorg;
   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);

   if (ar_expectedSeq.size() == 0)
   {
      if (seq.size() < 3)
      {
         WARNING("number of aircraft (" << seq.size() << ") read from " << inpFileName
                 << " is 0 ");
         return false;
      }
   }
   else
   {
      if (seq.size() != ar_expectedSeq.size())
      {
         WARNING("number of aircraft (" << seq.size() << ") read from " << inpFileName
                 << " different from expected one " << ar_expectedSeq.size());
         return false;
      }
   }
   vector<Aircraft> bestSeq; 
   AStarAcft astarInst(ar_qualiFunc);
   K_R_TIMER timer;
   timer.start_timer();
   bool retValue = astarInst.getBestSequence(seq, bestSeq);
   double ges_zeit = timer.stop_timer();
   cout << "Needed time for " << bestSeq.size() << " elements: "
      << ges_zeit << ", total quality value: " << astarInst.getEvalForBestSeq() << endl;

   if (ad_expValue > 0)
   {
      retValue = (fabs(ad_expValue - astarInst.getEvalForBestSeq()) < 0.1) && retValue;
   }
   else
   {
      // also better values are allowed
      ad_expValue = -ad_expValue;
      if (ad_expValue  > astarInst.getEvalForBestSeq() + 0.1)
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
   for (auto& iter : ar_expectedSeq)
   {
      retValue = retValue && bestSeq.at(i).getCallsign() == iter;
      if (false == retValue)
      {
         cout << "diff expected " << iter << "\n";
         break;
      }
      ++i;
   }

   if (retValue == false)
   {
      printSequenceTo(cout, bestSeq, astarInst.getEvalForBestSeq());
      cout << "expected Value is: " << ad_expValue
         << ", we got " << astarInst.getEvalForBestSeq() << endl;
   }

   retValue = retValue && ges_zeit <= 60.0;

   return retValue;

} // bool testSeqValueAndSequenceAcft



#ifndef WETTBEWERB_ACFT


/**
Dieser Test implementiert das Abnahmekritierium:
Sie bekommen eine Flugzeugdatei mit 9 Flugzeugen und
optimieren die Flugzeugdatei nach verschiedenen Kriterien.
Nur Earliest-Zeit, nur Latest-Zeit, nur Letzte Planzeit
und Kombinationen aus diesen.
Alle Ergebnisse richtig innerhalb von jeweils 60 Sekunden Rechenzeit
*/
bool test9AircraftWithDifferentWeightedCriteria()
{
   LOG_FUNCTION(CLASS_LT, "test9AircraftWithDifferentWeightedCriteria", "");
   bool retValue = true;

   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   cout << endl << endl;
   return retValue;
}// test9AircraftWithDifferentWeightedCriteria


/**
Dieser Test implementiert das Abnahmekritierium:
Sie bekommen einfache Flugzeugdateien mit 5-9 Flugzeugen.
Alle Ergebnisse richtig jeweils innerhalb von 60 Sekunden Rechenzeit
*/
bool test5to9SimpleAcftFiles()
{
   bool retValue = true;
   QualityFuncAircraft qualiFuncOnlyEarly;
   qualiFuncOnlyEarly.SetCriteriaWeights(1.0, 0.0, 0.0);

   retValue = retValue && testSeqValueAndSequenceAcft(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbAircraft2018/Aircraft5.txt",
      { "BAW766", "AF8797", "IBE765", "AF458",  "AF345" },
      33750.0, qualiFuncOnlyEarly);

   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   return retValue;

} // test5to9SimpleAcftFiles()

bool test10to16SimpleAcftFiles()
{
   bool retValue = true;
   QualityFuncAircraft qualiFuncOnlyEarly;
   qualiFuncOnlyEarly.SetCriteriaWeights(1.0, 0.0, 0.0);

   retValue = retValue && testSeqValueAndSequenceAcft(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbAircraft2018/Aircraft5.txt",
      { "BAW766", "AF8797", "IBE765", "AF458",  "AF345" },
      33750.0, qualiFuncOnlyEarly);

   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";


   return retValue;

}//  test10to16SimpleAcftFiles()

bool test5to15DifficultAcftFiles()
{
   QualityFuncAircraft qualiFuncAllEqual;
   qualiFuncAllEqual.SetCriteriaWeights(1.0, 1.0, 1.0);
   bool retValue = true;

   cout.precision(10);

   retValue = testSeqValueAndSequenceAcft(
      FILE_BASE_DIR_NAME + "./Data/WettbewerbAircraft2018/Complex/Aircraft5Cpx.txt",
      {  },
      -74831, qualiFuncAllEqual) && retValue;

   cout << "nur Geduld: In Kuerze erhalten Sie die echte Implementierung dieser Funktion\n";

   return retValue;

}// test5to15DifficultAcftFiles()


#endif
