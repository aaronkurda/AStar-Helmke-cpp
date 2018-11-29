/* 18-September-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung und TakeSelect-Algorithmus
   Quellcodedatei des Tests für die Aircraft-Tests
*/

#include "SearchAircraftTest.h"
#include <iomanip>
#include <vector>

#include "TakeSelect.h"
#include "zeitmess.h"


//#include "FileInput.h"
#include "Aircraft.h"
#include "EvalSeqAircraft.h"
#include "QualityFuncAircraft.h"

#include "QualityFuncAircraft.h"
#include "BaseDir.h"
#include "testMakros.h"

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;
#ifdef LOGTRACE_DEBUG
static const char  g_DEBUGns[] = "SCHEDULER_TEST";
static const char  g_DEBUGcl[] = "SearchAircraftTest";
#endif



ofstream g_outFile(FILE_BASE_DIR_NAME + "./Data/resultsAcft.txt", ios::out);



/** Die 9 Flugzeuge werden aus der Datei Data/Aircraft9Res.txt gelesen.
Für dort stehende Reihenfolge wird die Sequenz bewertet und 
mit den Erwartungen verglichen.
*/
bool testEvaluateSequenceAcft()
{
   LOG_FUNCTION(g_DEBUGcl, "testEvaluateSequenceAcft", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9.txt");

   Aircraft vorg;
   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);

   double seqValue = evaluateSequenceFunc(seq, vorg, evaluateTimeSquareAcft, 2.0);
   return DoubleEqual(1554520.0, seqValue, 0.1);
} // testEvaluateSequenceAcft



/** Die 9 Flugzeuge werden aus der Datei Data/Aircraft9Res.txt gelesen.
Diese Reihenfolge wird mit unterschiedlichen Parametersaetzen gewichtet
und bewertet und jeweils mit den Erwartungen verglichen.
*/
bool testEvalAcftSeqByDiffWeights()
{
   LOG_FUNCTION(g_DEBUGcl, "testEvalAcftSeqByDiffWeights", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9SameQuali_2.txt");

   Aircraft vorg;
   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);
   // in ein Array von Zeigern auf die einzelnen Aircraft umwandeln
   vector<Aircraft*> acftPtrVect;
   for (auto& iter : seq)
   {
      acftPtrVect.push_back(&(iter));
   }

   double seqValue = 0.0;
   bool retValue = true;

   QualityFuncAircraft qualiFuncOnlyLast;
   qualiFuncOnlyLast.SetCriteriaWeights(0.0, 0.0, 1.0);
   seqValue = qualiFuncOnlyLast.CalcValueOfSeq1Pred(acftPtrVect, vorg);
   retValue = DoubleEqual(443280.0, seqValue, 0.1) && retValue;


   QualityFuncAircraft qualiFuncOnlyLatest;
   qualiFuncOnlyLatest.SetCriteriaWeights(0.0, 1.0, 0.0);
   seqValue = qualiFuncOnlyLatest.CalcValueOfSeq1Pred(acftPtrVect, vorg);
   retValue = DoubleEqual(1096.0, seqValue, 0.1) && retValue;


   QualityFuncAircraft qualiFuncOnlyEarly;
   qualiFuncOnlyEarly.SetCriteriaWeights(1.0, 0.0, 0.0);
   seqValue = qualiFuncOnlyEarly.CalcValueOfSeq1Pred(acftPtrVect, vorg);
   retValue = DoubleEqual(697785.0, seqValue, 0.1) && retValue;


   QualityFuncAircraft qualiFuncAllEqual;
   qualiFuncAllEqual.SetCriteriaWeights(1.0, 1.0, 1.0);
   seqValue = qualiFuncAllEqual.CalcValueOfSeq1Pred(acftPtrVect, vorg);
   retValue =  DoubleEqual(380720.333, seqValue, 0.1) && retValue;

   return retValue;
} // 






/** Die 9 Flugzeuge werden aus der Datei Data/Aircraft9Res.txt gelesen.
   Hierfür wird durch die Funktion getBestSequenceFunc verwendet, die
   beste Sequenz ermittelt und mit den Erwartungen verglichen.
*/
bool testGetBestSequenceAircraft9()
{
   LOG_FUNCTION(g_DEBUGcl, "testGetBestSequenceAircraft9", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9.txt");
   string expResFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9Res.txt");

   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);

   vector<Aircraft> bestSeq(seq.size());
   Aircraft prevAcft;

   char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
   getBestSequenceFunc(seq, bestSeq, prevAcft, evaluateTimeSquareAcft, -1);
   double ges_zeit = timer. stop_timer();   // Timer stoppen
   cout << "Need time: " << ges_zeit << "\n";
   printSequenceTo(cout, bestSeq);

   bool retValue = true;
   return retValue;

} // testGetBestSequenceAircraft9


/** Die 9 Flugzeuge werden aus der Datei 
   /Data/Aircraft9.txt gelesen.
   Hierfür wird die beste Sequenz ermittelt 
   und mit den Erwartungen verglichen.
   Als Bewertungsfunktion wird ar_qualiFunc verwendet.
*/
bool testAircraft9WithQualiFunc
   (
   QualityFuncAircraft& ar_qualiFunc
   )
{
   LOG_FUNCTION(g_DEBUGcl, "testAircraft9WithQualiFunc", "");

   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9.txt");
   string expResFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9Res.txt");

   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);

   vector<Aircraft> bestSeq(seq.size());
   Aircraft prevAcft;

   char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
   double quali = getBestSequenceQualiClass(seq, bestSeq, prevAcft, ar_qualiFunc);
   double ges_zeit = timer. stop_timer();   // Timer stoppen
   cout << "Need time: " << setw(8) << ges_zeit 
      << ", Quality: " << setw(8) << quali << "\n";
   printSequenceTo(cout, bestSeq);

   bool retValue = true;
   return retValue;

} //testAircraft9WithQualiFunc


/** 9 Flugzeuge werden zum Testen verwendet
   Als Bewertungsfunktion wird nur das Early-Kriterium beachtet.
*/
bool testAircraft9WithQualityClassEarly()
{
   LOG_FUNCTION(g_DEBUGcl, "testAircraft9WithQualityClassEarly", "");

   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(1.0, 0.0, 0.0);
   return testAircraft9WithQualiFunc(qualiFunc);

} //testAircraft9WithQualityClassEarly


/** 9 Flugzeuge werden zum Testen verwendet
   Als Bewertungsfunktion werden alle 3 Kriterien gleich gewichtet.
*/
bool testAircraft9WithQualityClass()
{
   LOG_FUNCTION(g_DEBUGcl, "testAircraft9WithQualityClass", "");

   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(1.0/3.0, 1.0/3.0, 1.0/3.0);
   return testAircraft9WithQualiFunc(qualiFunc);

} //testAircraft9WithQualityClass

/** 9 Flugzeuge werden zum Testen verwendet
   Als Bewertungsfunktion werden nur die Abweichung von Earliest und von
   der Latest-Zeit gewichtet.
*/
bool testAircraft9WithQualityClassEarlyLate()
{
   LOG_FUNCTION(g_DEBUGcl, "testAircraft9WithQualityClassEarlyLate", "");

   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(0.5, 0.5, 0.0);

   return testAircraft9WithQualiFunc(qualiFunc);

} //testAircraft9WithQualityClass



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Flugzeuge werden aus der Datei inpFileName eingelesen. Der Take-Select-
  Algorithmus wird mit den Parameter select und take hierfür aufgerufen.
  Als Bewertungsfunktion wird ar_qualiFunc verwendet.
  Falls qualiValue kleiner 0 ist, wird die ermittelte Sequenz mit
  dem Inhalt in der Datei expResFileName verglichen. Andernfalls
  wird geprüft, ob der geliefert Qualitätswert kleiner gleich qualityValue
  ist.
  Die Ergebnisseqeunz und die gemessene Laufzeit wird eine Datei ausgegeben.
  Ist printToCout true, erfolgt die Ausgabe der Sequenzbewertung und der
  Laufzeit zusätzlich auf den Bildschrim.
*/

static bool openFileAndTestTakeSelectFunc
   (
   string inpFileName,
   string expResFileName,
   string stratName,
   int select,
   int take,
   double qualityValue,
   QualityFuncAircraft& ar_qualiFunc,
   bool printToCout=true
   )
{
   LOG_FUNCTION(g_DEBUGcl,"openFileAndTestTakeSelectFunc","");
   bool retValue = true;

   vector<Aircraft> seq;
   readElemsFromFile(inpFileName, seq);
   vector<Aircraft> bestSeq(seq.size());

   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);

   char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
   double quali = strategy.GetBestQualiClass(seq, bestSeq, ar_qualiFunc);
   double ges_zeit = timer. stop_timer();   // Timer stoppen

    g_outFile << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
       << ", Quality: " << setw(10) << quali << "\n";
    if (printToCout)
    {
       cout << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
          << ", Quality: " << setw(10) << quali << "\n";
    }

   if (qualityValue < 0)
   {
      vector<Aircraft> expResult;
      readElemsFromFile(expResFileName, expResult);

      retValue = bestSeq.size() == (expResult.size());
      if (retValue)
      {
         for (int i=0; i < static_cast<int>(bestSeq.size()); ++i)
         {
            if ((bestSeq[i].getIdentifier() != expResult[i].getIdentifier() ) ||
               (bestSeq[i].getTargetTime() != expResult[i].getTargetTime() ) )
            {
               g_outFile << "Abweichung bei " << bestSeq[i].getIdentifier()
                  << "(" << bestSeq[i].getTargetTime()
                  << "), " << expResult[i].getIdentifier()
                  << "(" << expResult[i].getTargetTime() << ")\n";
               retValue=false;
            }
         }// for
      }// if
   }
   else
   {
      retValue = quali  <= qualityValue;
   }
   printSequenceTo(g_outFile, bestSeq);
   g_outFile << "Bewertung: " << quali << endl;
   return retValue;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 9 Flugzeuge aus der Testdatei einlesen und Prüfung, ob der TakeSelect-Algorithmus
   mit verschiedenen Parametern das erwartete Ergebnis liefert.
   Zur Bewertung der Sequenzgüte wird die Klasse ar_qualiFunc herangezogen.
   Nur beim letzten und vorletzten Aufruf von Take/Select (8/2 und 9/2
   Fenster) wird geprüft, ob das Ergebnis kleiner gleich expBestValue ist.
   Ansonsten wird nur geprüft, ob die ermittelte Sequenz mit
   ihrer Bewertung kleiner als expMaxValue ist.
   Ist dieses alles erfüllt, wird true geliefert.
   Zum Schluss wird der TakeSelect-Algorithmus mit einem 9/2-Fenster
   10mal hintereinander ausgeführt und die mittlere Laufzeit auf dem
   Bildschirm ausgegeben.
*/
bool testScheduleAircraft9EvalFunc
   (
   string outputText,
   double expMaxValue,
   double expBestValue,
   QualityFuncAircraft& ar_qualiFunc
   )
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testScheduleGuest9EvalFunc","");

   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9.txt");
   string expResFileName(FILE_BASE_DIR_NAME + "./Data/Aircraft9Res.txt");
   bool retValue=true;

   g_outFile << outputText << "\n";
   g_outFile << "********************************************\n";
   cout << outputText << "\n";
   cout << "********************************************\n";

   // Take-Select-Algorithmus mit ansteigender Fenstergröße
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect4/2", 4, 2, expMaxValue, ar_qualiFunc) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect6/2", 6, 2, expMaxValue, ar_qualiFunc) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect7/2", 7, 2, expMaxValue, ar_qualiFunc) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect8/3", 8, 3, expMaxValue, ar_qualiFunc) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect8/2", 8, 2, expBestValue, ar_qualiFunc) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect9/2", 9, 2, expBestValue, ar_qualiFunc) && retValue;

   // Laufzeitmeesung
   ABS_ZEITMESS_SCHL(10, "Zeitmessung 9/2 Take/Select 9 Gäste, vollständige Permuation", 
      cout,   
   {retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect9/2", 9, 2, expBestValue, ar_qualiFunc, false) 
        && retValue;}
      );
   return retValue;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 9 Flugzeuge aus der Testdatei einlesen und Prüfung, ob der TakeSelect-Algorithmus
   mit verschiedenen Parametern das erwartete Ergebnis liefert.
   if we have quadratic evaluation function (longer waiting times are bad).
   Early time, late time and last schedule are weighted equally.
*/
bool testTakeSelectAcftSameQuali()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testTakeSelectAcftSameQuali","");
   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(1.0/3.0, 1.0/3.0, 1.0/3.0);
   return testScheduleAircraft9EvalFunc("9 LFZ mit quadr. gleicher Bewertungsfunktion",
	   1000000.0, 393325.5, qualiFunc);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 9 Flugzeuge aus der Testdatei einlesen und Prüfung, ob der TakeSelect-Algorithmus
   mit verschiedenen Parametern das erwartete Ergebnis liefert.
   if we have quadratic evaluation function (longer waiting times are bad).
   If only consider earliest times.
*/
bool testTakeSelectAcftQualiEarly()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testTakeSelectAcftQualiEarly","");
   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(1.0, 0.0, 0.0);
   return testScheduleAircraft9EvalFunc("9 LFZ mit quadr.Bewertungsfunktion (nur early)",
	   1000000.0, 642284.0, qualiFunc);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 9 Flugzeuge aus der Testdatei einlesen und Prüfung, ob der TakeSelect-Algorithmus
   mit verschiedenen Parametern das erwartete Ergebnis liefert.
   Als Bewertungsfunktion werden nur die Abweichung von Earliest und von
   der Latest-Zeit gewichtet.
*/
bool testTakeSelectAcftQualiEarlyLate()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testTakeSelectAcftQualiEarlyLate","");
   QualityFuncAircraft qualiFunc;
   qualiFunc.SetCriteriaWeights(10.0, 10.0, 0.0);
   return testScheduleAircraft9EvalFunc("9 LFZ mit quadr.Bewertungsfunktion (early/Late)",
	   1000000.0, 321142.0, qualiFunc);
}


